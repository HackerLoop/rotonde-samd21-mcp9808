/*
 * mcp9808.c
 *
 * Created: 21/01/2016 20:53:14
 *  Author: GroundLayer
 */ 

#include "modules/drivers/mcp9808.h"
#include "modules/modules/mcp9808.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "MCP9808_TEMP"
#define MODULE_DATA_SIZE 1
#define MODULE_TEMP "temperature"

data_name MCP9808_names[MODULE_DATA_SIZE] = { MODULE_TEMP };
data_type MCP9808_types[] = { DOUBLE };

void MCP9808_tick(void);
definition *MCP9808_def;

void MCP9808_module_init(void) {
	MCP9808_init();
	MCP9808_def = register_definition(EVENT , MODULE_NAME , MCP9808_names , MCP9808_types , MODULE_DATA_SIZE , NULL, MCP9808_tick , 1000);
}

void MCP9808_tick(void) {
	double temp;
	
	if(MCP9808_read_temp_C(&temp)) {
		instance inst = create_instance(MODULE_NAME);
		if(inst.def != NULL && inst.values != NULL){
			(*(double*) inst.values[0]) = temp;
			portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
			if(xStatus != pdPASS){
				release_instance(inst);
			}
		}
	}
}