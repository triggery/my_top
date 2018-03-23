#include <stdio.h>
#include <unistd.h>

#define WAIT_SEC 2

typedef struct {
	long unsigned int user;
	long unsigned int nice;
	long unsigned int sys;
	long unsigned int idle;
	long unsigned int iowait;
	long unsigned int irq;
	long unsigned int softirq;
	long unsigned int steal;
} CPU_STATE_TYPE;

int getCpuState(CPU_STATE_TYPE *cpu) {
	FILE *stat;
	stat = fopen("/proc/stat", "r");
	if(!stat) {
		printf("Stat open failed!");
		return 1;
	}
	fscanf(stat, "cpu %lu %lu %lu %lu %lu %lu %lu %lu", &cpu->user, &cpu->nice, &cpu->sys, &cpu->idle, &cpu->iowait, &cpu->irq, &cpu->softirq, &cpu->steal);
	fclose(stat);
	return 0;
}

int main(void)
{	
	CPU_STATE_TYPE cpu_state;
	long unsigned int prev_idle = 0, prev_total = 0, idle, total, diff_idle, diff_total, diff_usage;
	//int i = 0;
	while(/*i < WAIT_SEC*/1) 
	{
		getCpuState(&cpu_state);
		//printf("%lu %lu %lu %lu %lu %lu %lu %lu\n", cpu_state.user, cpu_state.nice, cpu_state.sys, cpu_state.idle, cpu_state.iowait, cpu_state.irq, cpu_state.softirq, cpu_state.steal);
		idle = cpu_state.idle;
		total = cpu_state.user + cpu_state.nice + cpu_state.sys + cpu_state.idle + cpu_state.iowait + cpu_state.irq + cpu_state.softirq + cpu_state.steal;
		diff_idle = idle - prev_idle;
		diff_total = total - prev_total;
		diff_usage = (1000*(diff_total-diff_idle)/diff_total + 5)/10;

		printf("%lu \n", diff_usage);

		prev_total = total;
		prev_idle = idle;
		/*i++;
		if(i < WAIT_SEC)*/
			sleep(1);
	}
	//printf("%lu \n", diff_usage);
	return 1;
}
