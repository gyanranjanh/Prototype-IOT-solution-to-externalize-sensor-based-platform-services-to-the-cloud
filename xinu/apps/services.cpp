#include <xinu_cpp.h>
#include <services.h>
#include <ledservice.h>
#include <thermometerservice.h>

static pid32 ledService;
static pid32 thermometerService;

void initialize_services() {
	kprintf("initialize_services>>\n");
    resched_cntl(DEFER_START);
    ledService = create((void *)ledServiceFn, SERVICE_STD_STK_SIZE, 
								SERVICE_STD_THD_PRIO, "ledService", 0);
    thermometerService = create((void *)thermometerServiceFn, SERVICE_STD_STK_SIZE,
								SERVICE_STD_THD_PRIO, "thermometerService", 0);
    resume(ledService);
    resume(thermometerService);
    resched_cntl(DEFER_STOP);
}

void deinitialize_services() {
	kprintf("deinitialize_services>>\n");
	kill(ledService);
	kill(thermometerService);
}


