#define PRINT_NL serial->write('\n')
#define PRINT_TAB serial->write('\t')
#define PRINT_VAR(V) serial->print(V)
#define PRINT_CELL(V) PRINT_VAR(V); PRINT_TAB

void print_var(HardwareSerial *serial){
	//extern App app;
	//serial->print("app_config.id: ");serial->println(app.config.id);
	//serial->print("app_config.serial_device: ");serial->println(app.config.serial_device);
	//serial->print("app_config.serial_rate: ");serial->println(app.config.serial_rate);
	//serial->print("app_config.serial_config: ");serial->println(app.config.serial_config);
	
	//serial->println("channel initial:");
	//serial->println("id\tenable\tpwm_period\tpwm_resolution\tpwm_dcmin\tpwm_dcmax\tsec_enable\tsec_out\tsec_timeout");
	//extern Channel channels[CHANNEL_COUNT];
	//for(size_t i = 0; i<CHANNEL_COUNT; i++){
		//Channel *channel = &channels[i];
		//PRINT_CELL(channel->id);
		//PRINT_CELL(channel->enable);
		//PRINT_CELL(channel->pwm.period);
		//PRINT_CELL(channel->pwm.resolution);
		//PRINT_CELL(channel->pwm.duty_cycle_min);
		//PRINT_CELL(channel->pwm.duty_cycle_max);
		//PRINT_CELL(channel->secure.enable);
		//PRINT_CELL(channel->secure.output);
		//PRINT_CELL(channel->secure.tmr.interval);
		//PRINT_NL;
	//}
	//serial->println("\nchannel rutime:");
	//serial->println("id\tstate\terror\tsec_state");
	//for(size_t i = 0; i<CHANNEL_COUNT; i++){
		//Channel *channel = &channels[i];
		//PRINT_CELL(channel->id);
		//PRINT_CELL(channel_getStateStr(channel));
		//PRINT_CELL(channel_getErrorDescr(channel));
		//PRINT_CELL(secure_getStateStr(&channel->secure));
		//PRINT_NL;
	//}
}

