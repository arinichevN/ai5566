int check_float (double v) {
	if(isinf(v)) return 0;
	if(isnan(v)) return 0;
	return 1;
}
