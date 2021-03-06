
module TimerSoC (
	buttons_export,
	clk_clk,
	displays_export,
	gpio_in_export,
	gpio_out_export,
	leds_export,
	reset_reset_n,
	switches_export,
	uart_rxd,
	uart_txd);	

	input	[1:0]	buttons_export;
	input		clk_clk;
	output	[31:0]	displays_export;
	input	[7:0]	gpio_in_export;
	output	[7:0]	gpio_out_export;
	output	[9:0]	leds_export;
	input		reset_reset_n;
	input	[1:0]	switches_export;
	input		uart_rxd;
	output		uart_txd;
endmodule
