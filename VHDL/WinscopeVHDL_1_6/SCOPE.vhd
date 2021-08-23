library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity TRIGG_COMP is
	port (
		CLK		: in std_logic;
		RESET	: in std_logic;
		CHAN_IN	: in std_logic_vector(7 downto 0);
		LEVEL	: in std_logic_vector(7 downto 0);
		POS_EDGE: out std_logic;
		NEG_EDGE: out std_logic
	);
end TRIGG_COMP;

architecture BEHAV of TRIGG_COMP is
	signal COMP		: std_logic;
	signal COMP_d	: std_logic;
begin
	process(CLK, RESET)
	begin
		if (RESET = '0') then
			POS_EDGE <= '0';
			NEG_EDGE <= '0';
			COMP <= '0';
			COMP_d <= '0';
		elsif rising_edge(CLK) then
			COMP_d <= COMP;
			if (unsigned(CHAN_IN) > unsigned(LEVEL)) then
				COMP <= '1';
			else
				COMP <= '0';
			end if;
			if (COMP = '1') and (COMP_d = '0') then POS_EDGE <= '1';
			else POS_EDGE <= '0';
			end if;
			if (COMP = '0') and (COMP_d = '1') then NEG_EDGE <= '1';
			else NEG_EDGE <= '0';
			end if;
		end if;
	end process;
end behav;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity MAIN is
	port (
		CLK			: in std_logic;
		RESET		: in std_logic;

		GEN_SMP_CLK	: out std_logic;
		SMP_CLK		: in std_logic;
		EXT_CLK		: in std_logic;

		ALE			: in std_logic;
		DATA		: inout std_logic_vector(7 downto 0);
		CPU_RD_N	: in std_logic;
		CPU_WR_N	: in std_logic;
		CPU_INT_N	: out std_logic;
		D12_CS_N	: out std_logic;
		RD_N		: out std_logic;
		WR_N		: out std_logic;
		DMAREQ		: in std_logic;
		DMACK		: out std_logic;
		EOT_N		: out std_logic;

		MEM_E0_N	: out std_logic;
		MEM_E1_N	: out std_logic;
		MEM_G_N		: out std_logic;
		MEM_BS0_N	: out std_logic;
		MEM_BS1_N	: out std_logic;
		MEM_BS2_N	: out std_logic;
		MEM_BS3_N	: out std_logic;
		ADDR_COUNT	: out std_logic_vector(18 downto 0);

		PRBA_IN		: in std_logic_vector(7 downto 0);
		PRBB_IN		: in std_logic_vector(7 downto 0);
		PODP_IN		: in std_logic_vector(7 downto 0);
		PODQ_IN		: in std_logic_vector(7 downto 0);

		PRBA_GAIN	: out std_logic_vector(3 downto 0);
		PRBB_GAIN	: out std_logic_vector(3 downto 0);
		PRBA_AC		: out std_logic;
		PRBB_AC		: out std_logic;
		PRBA_GND	: out std_logic;
		PRBB_GND	: out std_logic;

		LEDS		: out std_logic_vector(7 downto 0)
	);
end MAIN;

architecture STRCTNBEHAV of MAIN is
	component CLKGEN is
		port (
			CLK		: in std_logic;
			RESET	: in std_logic;
			FRESEL	: in unsigned(4 downto 0);
			CLK_OUT	: out std_logic
		);
	end component CLKGEN;


	component TRIGG_COMP is
		port (
			CLK		: in std_logic;
			RESET	: in std_logic;
			CHAN_IN	: in std_logic_vector(7 downto 0);
			LEVEL	: in std_logic_vector(7 downto 0);
			POS_EDGE: out std_logic;
			NEG_EDGE: out std_logic
		);
	end component;

	signal ADDRESS		: std_logic_vector(7 downto 0);
	signal I_CH_A_CE_n	: std_logic;
	signal I_CH_B_CE_n	: std_logic;
	signal TLR_WE_n		: std_logic;
	signal TPRH_WE_n	: std_logic;
	signal TPRL_WE_n	: std_logic;
	signal SR_WE_n		: std_logic;
	signal FSR_WE_n		: std_logic;
	signal TRIGG_LEVEL	: std_logic_vector(7 downto 0);
	signal TRIGG_POS	: unsigned(12 downto 0);
	signal A_POS_EDGE	: std_logic;
	signal A_NEG_EDGE	: std_logic;
	signal B_POS_EDGE	: std_logic;
	signal B_NEG_EDGE	: std_logic;
	signal TRIGG		: std_logic;
	signal TRIGG_SOURCE	: std_logic;
	signal TRIGG_EDGE	: std_logic;
	signal POS_COUNTER	: unsigned(12 downto 0);
	signal ARMED		: std_logic;
	signal OLD_MEASURE	: std_logic;
	signal READ_A 		: std_logic;
	signal READ_B 		: std_logic;
	signal STOP_MEASURE : std_logic;
	signal I_MEASURE	: std_logic;
	signal I_ADR_CNT	: unsigned(12 downto 0);
	signal I_CH_A_OE_n	: std_logic;
	signal I_CH_B_OE_n	: std_logic;
	signal FRESEL		: unsigned(4 downto 0);

begin
	CLKGEN1 : CLKGEN port map (
			CLK => CLK,
			RESET => RESET,
			FRESEL => FRESEL,
			CLK_OUT => SMP_CLK_OUT
		);

	ADR_LATCH : process(ALE, RESET)
	begin
		if (RESET = '0') then
			ADDRESS <= (others => '0');

		elsif falling_edge(ALE) then
			ADDRESS <= DATA;
		end if;
	end process;

	ADR <= ADDRESS(1 downto 0);
	DATA <= "ZZZZZZZZ";

	DAC_WE_n <= '0' when ADDRESS(7 downto 4) = x"1" and WR_n = '0' else '1';
	I_CH_A_OE_n <= '0' when ADDRESS(7 downto 4) = x"2" and RD_N = '0' else '1';
	I_CH_B_OE_n <= '0' when ADDRESS(7 downto 4) = x"3" and RD_N = '0'  else '1';
	I_CH_A_CE_n <= '0' when ADDRESS(7 downto 4) = x"4" and RD_N = '0'  else '1';
	I_CH_B_CE_n <= '0' when ADDRESS(7 downto 4) = x"5" and RD_N = '0'  else '1';
	TLR_WE_n <= '0' when ADDRESS(7 downto 4) = x"6" and WR_n = '0' else '1';
	TPRH_WE_n <= '0' when ADDRESS(7 downto 4) = x"7" and WR_n = '0' else '1';
	TPRL_WE_n <= '0' when ADDRESS(7 downto 4) = x"8" and WR_n = '0' else '1';
	SR_WE_n <= '0' when ADDRESS(7 downto 4) = x"9" and WR_n = '0' else '1';
	FSR_WE_n <= '0' when ADDRESS(7 downto 4) = x"A" and WR_n = '0' else '1';

	CH_A_OE_n <= I_CH_A_OE_n;
	CH_B_OE_n <= I_CH_B_OE_n;
	CH_A_CE_n <= I_CH_A_CE_n when I_MEASURE = '0' else SMP_CLK;
	CH_B_CE_n <= I_CH_B_CE_n when I_MEASURE = '0' else SMP_CLK;
	
	REGISTERS : process(CLK, RESET)
	begin
		if (RESET = '0') then
			TRIGG_LEVEL <= (others => '0');
			TRIGG_POS <= to_unsigned(256, 13);
			TRIGG_SOURCE <= '0';
			TRIGG_EDGE <= '0';
			I_MEASURE <= '0';
			FRESEL <= (others => '0');
		elsif rising_edge(CLK) then
			if (TLR_WE_n = '0') then
				TRIGG_LEVEL <= DATA;
			end if;
			if (TPRH_WE_n = '0') then
				TRIGG_POS(12 downto 8) <= unsigned(DATA(4 downto 0));
			end if;
			if (TPRL_WE_n = '0') then
				TRIGG_POS(7 downto 0) <= unsigned(DATA);
			end if;
			if (SR_WE_n = '0') then
				TRIGG_SOURCE <= DATA(0);
				TRIGG_EDGE <= DATA(1);
				I_MEASURE <= DATA(2);
			end if;
			if (FSR_WE_n = '0') then
				FRESEL <= unsigned(DATA(4 downto 0));
			end if;
		end if;
	end process REGISTERS;

	MEASURE_N <= not I_MEASURE;
	MEASURE <= I_MEASURE;

	CH_A_TRIGG_COMP : TRIGG_COMP port map (
		CLK => SMP_CLK,
		RESET => RESET,
		CHAN_IN => CHA_IN,
		LEVEL => TRIGG_LEVEL,
		POS_EDGE => A_POS_EDGE,
		NEG_EDGE => A_NEG_EDGE
	);

	CH_B_TRIGG_COMP : TRIGG_COMP port map (
		CLK => SMP_CLK,
		RESET => RESET,
		CHAN_IN => CHB_IN,
		LEVEL => TRIGG_LEVEL,
		POS_EDGE => B_POS_EDGE,
		NEG_EDGE => B_NEG_EDGE
	);

	TRIGGER : process(SMP_CLK, RESET)
	begin
		if (RESET = '1') then
			TRIGG <= '0';
		elsif rising_edge(SMP_CLK) then
			TRIGG <= '0';															--Default assignment
			if (TRIGG_SOURCE = '0') then
				if (TRIGG_EDGE = '0') and (A_POS_EDGE = '1') then TRIGG <= '1';		--Channel A, Pos edge Trigg
				end if;
				if (TRIGG_EDGE = '1') and (A_NEG_EDGE = '1') then TRIGG <= '1';		--Channel A, Neg edge Trigg
				end if;
			else
				if (TRIGG_EDGE = '0') and (B_POS_EDGE = '1') then TRIGG <= '1';		--Channel B, Pos edge Trigg
				end if;
				if (TRIGG_EDGE = '1') and (B_NEG_EDGE = '1') then TRIGG <= '1';		--Channel B, Neg edge Trigg
				end if;
			end if;
		end if;
	end process;
		
	TRIGG_POS_COUNTER : process(SMP_CLK, RESET)
	begin
		if (RESET = '1') then
			STOP_MEASURE <= '1';
			ARMED <= '0';
			OLD_MEASURE <= '0';
			POS_COUNTER <= (others => '0');
		elsif rising_edge(SMP_CLK) then
			OLD_MEASURE <= I_MEASURE;
			if (STOP_MEASURE = '1') then
				if (OLD_MEASURE = '0') and (I_MEASURE = '1') then
					STOP_MEASURE <= '0';
					ARMED <= '1';
					POS_COUNTER <= TRIGG_POS;
				end if;
			else
				if (ARMED = '1') then
					if (TRIGG = '1') then
						ARMED <= '0';
					end if;
				else
					if (POS_COUNTER = to_unsigned(0, 13))then
						STOP_MEASURE <= '1';
					else
						POS_COUNTER <= POS_COUNTER - 1;
					end if;
				end if;
			end if;
		end if;
	end process;
	
	ADDRESS_COUNTER : process(SMP_CLK, RESET)
	begin
		if (RESET = '1') then
			I_ADR_CNT <= (others => '0');
			READ_A <= '0';
			READ_B <= '0';
		elsif rising_edge(SMP_CLK) then
			if (STOP_MEASURE = '0') then
				if (I_MEASURE = '1') then
					I_ADR_CNT <= I_ADR_CNT - 1;
				else
					if (I_CH_A_OE_n = '0') then READ_A <= '1';		--Maybe doesnt catch this, potential error
					end if;
					if (I_CH_B_OE_n = '0') then READ_B <= '1';		--Maybe doesnt catch this, potential error
					end if;
					if (READ_A = '1' ) and (READ_B = '1') then 
						READ_A <= '0';
						READ_B <= '0';
						I_ADR_CNT <= I_ADR_CNT - 1;
					end if;
				end if;
			end if;
		end if;
	end process;

	ADR_CNT <= I_ADR_CNT;

end STRCTNBEHAV;



