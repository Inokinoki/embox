/**
 * @file
 * @brief Software interrupts subsystem.
 *
 * @date 24.12.09
 * @author Anton Bondarev
 *         - Initial implementation
 * @author Eldar Abusalimov
 *         - Rewriting from scratch:
 *          - Implementing new interface
 *          - Introducing interrupt safety
 */

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#include <kernel/softirq.h>
#include <kernel/critical.h>
#include <hal/ipl.h>

static void softirq_dispatch(void);

CRITICAL_DISPATCHER_DEF(softirq_critical, softirq_dispatch,
		CRITICAL_SOFTIRQ_LOCK);

struct softirq_action {
	softirq_handler_t handler;
	void *data;
};

static struct softirq_action softirq_actions[SOFTIRQ_NRS_TOTAL];
static uint32_t softirq_pending;
static uint32_t softirq_handling;

int softirq_install(softirq_nr_t nr, softirq_handler_t handler, void *data) {
	ipl_t ipl;

	if (!softirq_nr_valid(nr)) {
		return -EINVAL;
	}

	ipl = ipl_save();
	softirq_actions[nr].handler = handler;
	softirq_actions[nr].data = data;
	ipl_restore(ipl);

	return 0;
}

int softirq_raise(softirq_nr_t nr) {
	ipl_t ipl;

	if (!softirq_nr_valid(nr)) {
		return -EINVAL;
	}

	ipl = ipl_save();
	softirq_pending |= (1 << nr);
	ipl_restore(ipl);

	critical_request_dispatch(&softirq_critical);

	return 0;
}

/**
 * Called by critical dispatching code with max IPL (all IRQ disabled).
 */
static void softirq_dispatch(void) {
	uint32_t pending;
	uint32_t handling;
	softirq_handler_t handler;
	void *data;

	critical_enter(CRITICAL_SOFTIRQ_HANDLER);

	while ((pending = softirq_pending)) {
		softirq_pending = 0;
		handling = 0x1;
		for (softirq_nr_t nr = 0; pending; pending >>= 1, handling <<= 1, ++nr) {
			if (!(pending & 0x1) || (softirq_handling & handling)) {
				continue;
			}

			softirq_handling |= handling;

			if ((handler = softirq_actions[nr].handler)) {
				data = softirq_actions[nr].data;

				ipl_enable();
				handler(nr, data);
				ipl_disable();
			}

			softirq_handling &= ~handling;
		}
	}

	critical_leave(CRITICAL_SOFTIRQ_HANDLER);
}
