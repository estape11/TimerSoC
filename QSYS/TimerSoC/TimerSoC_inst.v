	TimerSoC u0 (
		.buttons_export  (<connected-to-buttons_export>),  //  buttons.export
		.clk_clk         (<connected-to-clk_clk>),         //      clk.clk
		.displays_export (<connected-to-displays_export>), // displays.export
		.gpio_in_export  (<connected-to-gpio_in_export>),  //  gpio_in.export
		.gpio_out_export (<connected-to-gpio_out_export>), // gpio_out.export
		.leds_export     (<connected-to-leds_export>),     //     leds.export
		.reset_reset_n   (<connected-to-reset_reset_n>),   //    reset.reset_n
		.switches_export (<connected-to-switches_export>), // switches.export
		.uart_rxd        (<connected-to-uart_rxd>),        //     uart.rxd
		.uart_txd        (<connected-to-uart_txd>)         //         .txd
	);

