-------------------------------------------------------------------------------
-- File name    :  $Id: CONSTANTS.vhd,v 1.2 2003/06/12 09:09:07 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: CONSTANTS.vhd,v $
-- Revision 1.2  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CONSTANTS is
	port (
		MEM_BS0_N	:	out std_logic;
		MEM_BS1_N	:	out std_logic;
		MEM_BS2_N	:	out std_logic;
		MEM_BS3_N	:	out std_logic
	);
end CONSTANTS;

architecture BEHAV of CONSTANTS is
begin
	MEM_BS0_N <= '0';
	MEM_BS1_N <= '0';
	MEM_BS2_N <= '0';
	MEM_BS3_N <= '0';
end architecture BEHAV;
