-------------------------------------------------------------------------------
-- File name    :  $Id: ADDR_COUNT.vhd,v 1.3 2003/06/12 09:09:07 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: ADDR_COUNT.vhd,v $
-- Revision 1.3  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ADDR_COUNT_C is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		BUFFER_SIZE		: in unsigned(16 downto 0);
		ADDR_COUNT		: out unsigned(16 downto 0)
	);
end ADDR_COUNT_C;

architecture BEHAV of ADDR_COUNT_C is
	signal	CNT_I	: unsigned(16 downto 0);
	signal	CNT_I_D	: unsigned(16 downto 0);
begin
	process(SMP_CLK, RESET)
	begin
		if (RESET = '0') then
			CNT_I <= (others => '0');
			CNT_I_D <= (others => '0');
		elsif (rising_edge(SMP_CLK)) then
			if (CNT_I < BUFFER_SIZE) then
				CNT_I <= CNT_I + 1;
			else
				CNT_I <= (others => '0');
			end if;
			CNT_I_D <= CNT_I;
		end if;
	end process;

	ADDR_COUNT <= CNT_I_D;
end architecture BEHAV;
