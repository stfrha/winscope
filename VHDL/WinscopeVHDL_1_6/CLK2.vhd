-------------------------------------------------------------------------------
-- File name    :  $Id: CLK2.vhd,v 1.5 2003/06/12 12:30:35 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: CLK2.vhd,v $
-- Revision 1.5  2003/06/12 12:30:35  frha
-- Added CAL_PULSE of 1kHz and TRIGG_LED
--
-- Revision 1.4  2003/06/12 09:33:21  frha
-- Added CAL_PULSE of 1kHz
--
-- Revision 1.3  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CLK_SYNT_SEL is
	port (
		CLK			: in std_logic;
		RESET		: in std_logic;
		CLK_CHANGE	: in std_logic;
		CLK_TOGGLE	: in std_logic_vector(23 downto 0);
		CLK_RESET	: in std_logic_vector(23 downto 0);

		EXT_CLK		: in std_logic;
		CLK_SELECT	: in std_logic;
		SAVE_LOAD_N	: in std_logic;
		INC_PULSE	: in std_logic;
		GEN_SMP_CLK	: out std_logic
	);
end CLK_SYNT_SEL;

architecture BEHAV of CLK_SYNT_SEL is
	signal SEL_CLK	: std_logic;
	signal GEN_CLK	: std_logic;
	signal CNT		: unsigned(23 downto 0);
	signal CLK_I	: std_logic;
	signal C30MHZ	: std_logic;
begin
	process(CLK, RESET)
	begin
		if (RESET = '0') then
			CNT <= (others => '0');
			CLK_I <= '1';
			C30MHZ <= '1';
		elsif (rising_edge(CLK)) then
			if (CLK_CHANGE = '1') then
				CNT <= (others => '0');
				CLK_I <= '0';
				if (CLK_RESET = x"000000") then
					C30MHZ <= '1';
				else
					C30MHZ <= '0';
				end if;
			else
				CNT <= CNT + 1;
				if (CNT = unsigned(CLK_TOGGLE)) then 
					CLK_I <= '1';
				end if;
				if (CNT = unsigned(CLK_RESET)) then
					CLK_I <= '0';
					CNT <= (others => '0');
				end if;
			end if;
		end if;
	end process;

	GEN_CLK <= CLK_I when (C30MHZ = '0') else CLK;
	SEL_CLK <= GEN_CLK when (CLK_SELECT = '0') else EXT_CLK;
	GEN_SMP_CLK <= SEL_CLK when SAVE_LOAD_N = '1' else INC_PULSE;

end architecture BEHAV;







library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ADDR_INC is
	port (
		CLK			: in std_logic;
		RESET		: in std_logic;

		LOAD_INC_PULSE	: in std_logic;
		INC_PULSE	: out std_logic;
		MEM_E0_N	: out std_logic;
		MEM_E1_N	: out std_logic
	);
end ADDR_INC;

architecture BEHAV of ADDR_INC is
	type ADDRINC_STATE_TYPE is (IDLE, DISABLE_MEM, NEG_EDGE, POS_EDGE);
	signal STATE	: ADDRINC_STATE_TYPE;
begin
	process(CLK, RESET)
	begin
		if (RESET = '0') then
			STATE <= IDLE;
			INC_PULSE <= '1';
			MEM_E0_N <= '0';		--Memories selected by default
			MEM_E1_N <= '0';		--Memories selected by default
		elsif (rising_edge(CLK)) then
			case STATE is
			when IDLE =>
				if (LOAD_INC_PULSE = '1') then
					STATE <= DISABLE_MEM;
					MEM_E0_N <= '1';
					MEM_E1_N <= '1';
				end if;
			when DISABLE_MEM =>
				STATE <= NEG_EDGE;
				INC_PULSE <= '0';
			when NEG_EDGE => 
				STATE <= POS_EDGE;
				INC_PULSE <= '1';
			when POS_EDGE =>
				if (LOAD_INC_PULSE = '1') then
					STATE <= NEG_EDGE;
					INC_PULSE <= '0';
				else
					STATE <= IDLE;
					MEM_E0_N <= '0';
					MEM_E1_N <= '0';
				end if;
			end case;
		end if;
	end process;
end architecture BEHAV;








library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CLK_GEN is
	port (
		CLK				: in std_logic;
		RESET			: in std_logic;
		CLK_CHANGE		: in std_logic;
		CLK_TOGGLE		: in std_logic_vector(23 downto 0);
		CLK_RESET		: in std_logic_vector(23 downto 0);

		EXT_CLK			: in std_logic;
		CLK_SELECT		: in std_logic;
		SAVE_LOAD_N		: in std_logic;
		GEN_SMP_CLK		: out std_logic;

		LOAD_INC_PULSE	: in std_logic;
		MEM_E0_N		: out std_logic;
		MEM_E1_N		: out std_logic;

		CAL_PULSE       : out std_logic
	);
end CLK_GEN;

architecture STRUCT of CLK_GEN is
	component CLK_SYNT_SEL is
		port (
			CLK			: in std_logic;
			RESET		: in std_logic;
			CLK_CHANGE	: in std_logic;
			CLK_TOGGLE	: in std_logic_vector(23 downto 0);
			CLK_RESET	: in std_logic_vector(23 downto 0);
	
			EXT_CLK		: in std_logic;
			CLK_SELECT	: in std_logic;
			SAVE_LOAD_N	: in std_logic;
			INC_PULSE	: in std_logic;
			GEN_SMP_CLK	: out std_logic
		);
	end component CLK_SYNT_SEL;
	
	component ADDR_INC is
		port (
			CLK			: in std_logic;
			RESET		: in std_logic;
	
			LOAD_INC_PULSE	: in std_logic;
			INC_PULSE	: out std_logic;
			MEM_E0_N	: out std_logic;
			MEM_E1_N	: out std_logic
		);
	end component ADDR_INC;

	signal INC_PULSE 	: std_logic;

	signal CAL_PULSE_I 	: std_logic;
    signal PULSE_COUNT  : unsigned (13 downto 0);

begin
	CSS : CLK_SYNT_SEL port map (
		CLK => CLK,
		RESET => RESET,
		CLK_CHANGE => CLK_CHANGE,
		CLK_TOGGLE => CLK_TOGGLE,
		CLK_RESET => CLK_RESET,

		EXT_CLK => EXT_CLK,
		CLK_SELECT => CLK_SELECT,
		SAVE_LOAD_N => SAVE_LOAD_N,
		INC_PULSE => INC_PULSE,
		GEN_SMP_CLK => GEN_SMP_CLK
	);

	AI : ADDR_INC port map (
		CLK => CLK,
		RESET => RESET,
		LOAD_INC_PULSE => LOAD_INC_PULSE,
		INC_PULSE => INC_PULSE,
		MEM_E0_N => MEM_E0_N,
		MEM_E1_N => MEM_E1_N
   	);

    CAL_PULSE_PROC : process(CLK, RESET)
    begin
        if (RESET = '0') then
            CAL_PULSE_I <= '0';
            PULSE_COUNT <= (others => '0');
        elsif (rising_edge(CLK)) then
            PULSE_COUNT <= PULSE_COUNT + 1;
            if (PULSE_COUNT = 15000) then           -- Pulse frequency of 1kHz
                PULSE_COUNT <= (others => '0');
                CAL_PULSE_I <= not CAL_PULSE_I;
            end if;
        end if;
    end process;

    CAL_PULSE <= CAL_PULSE_I;
            
end architecture STRUCT;

