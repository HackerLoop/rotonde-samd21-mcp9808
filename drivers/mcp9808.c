/*
 * mcp9808.c
 *
 * Created: 21/01/2016 20:34:15
 *  Author: GroundLayer
 */ 

#include "./mcp9808.h"

bool MCP9808_init(void){

	unsigned char data[2];

	data[0] = MCP9808_REG_MANUF_ID;
	HAL_I2C_write(MCP9808_I2CADDR_DEFAULT , 1 ,data );
	HAL_I2C_read(MCP9808_I2CADDR_DEFAULT , 2 , data);
	
	if(data[0] != 0x00 || data[1] != 0x54) {
		return false;
	}
	
	data[0] = MCP9808_REG_DEVICE_ID;
	HAL_I2C_write(MCP9808_I2CADDR_DEFAULT , 1 ,data );
	HAL_I2C_read(MCP9808_I2CADDR_DEFAULT , 2 , data);
	
	if(data[0] != 0x04 || data[1] != 0x000) {
		return false;
	}
	
	return true;
}

bool MCP9808_read_temp_C(double* temp){
	unsigned char data[2];
	data[0] = 	MCP9808_REG_AMBIENT_TEMP;
	
	if(HAL_I2C_write(MCP9808_I2CADDR_DEFAULT , 1 ,data ) != STATUS_OK) {
		return false;
	}
	if(HAL_I2C_read(MCP9808_I2CADDR_DEFAULT , 2 , data) != STATUS_OK) {
		return false;
	}
	
	uint16_t raw_temp;
	raw_temp = data[0] << 8 |  data[1];
	raw_temp = raw_temp & 0x0FFF;
	*temp = (float) raw_temp / 16.0;
	if(data[0] & 0x10) {
		*temp -= 256;
	}
	return true;
}
