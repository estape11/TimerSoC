	component TimerSoC is
		port (
			buttons_export  : in  std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			clk_clk         : in  std_logic                     := 'X';             -- clk
			displays_export : out std_logic_vector(31 downto 0);                    -- export
			gpio_in_export  : in  std_logic_vector(7 downto 0)  := (others => 'X'); -- export
			gpio_out_export : out std_logic_vector(7 downto 0);                     -- export
			leds_export     : out std_logic_vector(9 downto 0);                     -- export
			reset_reset_n   : in  std_logic                     := 'X';             -- reset_n
			switches_export : in  std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			uart_rxd        : in  std_logic                     := 'X';             -- rxd
			uart_txd        : out std_logic                                         -- txd
		);
	end component TimerSoC;

	u0 : component TimerSoC
		port map (
			buttons_export  => CONNECTED_TO_buttons_export,  --  buttons.export
			clk_clk         => CONNECTED_TO_clk_clk,         --      clk.clk
			displays_export => CONNECTED_TO_displays_export, -- displays.export
			gpio_in_export  => CONNECTED_TO_gpio_in_export,  --  gpio_in.export
			gpio_out_export => CONNECTED_TO_gpio_out_export, -- gpio_out.export
			leds_export     => CONNECTED_TO_leds_export,     --     leds.export
			reset_reset_n   => CONNECTED_TO_reset_reset_n,   --    reset.reset_n
			switches_export => CONNECTED_TO_switches_export, -- switches.export
			uart_rxd        => CONNECTED_TO_uart_rxd,        --     uart.rxd
			uart_txd        => CONNECTED_TO_uart_txd         --         .txd
		);

