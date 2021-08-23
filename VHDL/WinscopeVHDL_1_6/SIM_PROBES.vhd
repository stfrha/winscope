-------------------------------------------------------------------------------
-- File name    :  $Id: SIM_PROBES.vhd,v 1.4 2003/06/12 09:09:07 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: SIM_PROBES.vhd,v $
-- Revision 1.4  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity SIM_PROBES is
	port (
		CLK		: in std_logic;
		RESET	: in std_logic;
		CHA		: inout std_logic_vector(7 downto 0);
		CHA_EN_N: in std_logic;
		CHB		: inout std_logic_vector(7 downto 0);
		CHB_EN_N: in std_logic;
		PDP		: inout std_logic_vector(7 downto 0);
		PDP_EN_N: in std_logic;
		PDQ		: inout std_logic_vector(7 downto 0);
		PDQ_EN_N: in std_logic
	);
end SIM_PROBES;

architecture TESTBENCH of SIM_PROBES is
	signal		DIGCNT	: unsigned(7 downto 0);
	signal		COUNT	: real;
	signal      CHA_I	: std_logic_vector(7 downto 0);
	signal      CHB_I	: std_logic_vector(7 downto 0);
	signal      PDP_I	: std_logic_vector(7 downto 0);
	signal      PDQ_I	: std_logic_vector(7 downto 0);
	signal      CHA_M   : integer range 0 to 255;
	signal      CHB_M   : integer range 0 to 255;
    signal      TEST    : integer;
	constant	CLK_INC	: real := 33.3333e-9;	-- 1MHz
	constant	A_FREQ	: real := 1000000.0;	-- 1MHz
	constant	B_FREQ	: real := 300000.0;		-- 300kHz

begin
	process(CLK, RESET)
	begin
		if (RESET = '0') then
			DIGCNT <= (others => '0');
			COUNT <= 0.0;
			CHA_M <= 127;
			CHB_M <= 127;
			PDP_I <= (others => '0');
			PDQ_I <= (others => '0');
            TEST <= 0;
		elsif (rising_edge(CLK)) then
			COUNT <= COUNT + CLK_INC;
            TEST <=  integer(round( 120.0*sin(math_2_pi*COUNT*A_FREQ) + 127.0));
            CHA_M <= TEST;
--            CHB_M <= natural(round( 120.0*sin(math_2_pi*COUNT*B_FREQ) + 127.0));
			DIGCNT <= DIGCNT + 1;
			PDP_I <= std_logic_vector(DIGCNT);
			PDQ_I <= std_logic_vector(DIGCNT+5);
		end if;
	end process;

	CHA_I <= std_logic_vector(to_signed(CHA_M, 8));
	CHB_I <= std_logic_vector(to_signed(CHB_M, 8));

    CHA <= CHA_I when CHA_EN_N = '0' else (others => 'Z');
    CHB <= CHB_I when CHB_EN_N = '0' else (others => 'Z');
    PDP <= PDP_I when PDP_EN_N = '0' else (others => 'Z');
    PDQ <= PDQ_I when PDQ_EN_N = '0' else (others => 'Z');
end architecture TESTBENCH;

