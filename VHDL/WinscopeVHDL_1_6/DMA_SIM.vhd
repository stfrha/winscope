-------------------------------------------------------------------------------
-- File name    :  $Id: $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: $

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.ALL;
use std.textio.all;

entity D12_DMA_SIM is
	port (
		RESET   : in std_logic;
		TRIGGER	: in std_logic;
		DATA	: in std_logic_vector(7 downto 0);
		WR_N	: in std_logic;
		DMACK_N	: in std_logic;
		EOT_N	: in std_logic;
		DMAREQ	: out std_logic
	);
end D12_DMA_SIM;

architecture TESTBENCH of D12_DMA_SIM is
	signal	TWL		: time := 0 ns;
	signal	TWH		: time := 0 ns;
	signal	TDAH	: time := 0 ns;
	signal	TEL		: time := 0 ns;
	signal	TEH		: time := 0 ns;
	signal	TRH		: time := 0 ns;
	signal	DMAREQ_I	: std_logic;

    file OUTPUT_FILE : text open write_mode is "D12_DMA.TXT";

begin
	DMAREQ <= DMAREQ_I;

	process
	begin
		wait until falling_edge(WR_N);
		ASSERT ((now - TWL) >= 500 ns)
		REPORT "To short time between writes"
		SEVERITY ERROR;
		TWL <= now;
	end process;

	process
	begin
		wait until rising_edge(WR_N);
		ASSERT ((now - TRH) >= 120 ns)
		REPORT "To short between DMAREQ going high and WE going high"
		SEVERITY ERROR;
		TWH <= now;
	end process;

	process
	begin
		wait until rising_edge(DMAREQ_I);
		TRH <= now;
	end process;

	process
	begin
		wait until falling_edge(EOT_N);
		TEL <= now;
	end process;

	process
	begin
		wait until rising_edge(EOT_N);
		ASSERT ((now - TEL) >= 10 ns)
		REPORT "Not long enough pulsewidth of EOT_N"
		SEVERITY ERROR;
		TEH <= now;
	end process;

	process
	begin
		wait until rising_edge(DMACK_N);
		ASSERT ((now - TWH) < 130 ns)
		REPORT "To short time between WR going high and DMACK_N going high"
		SEVERITY ERROR;
		TDAH <= now;
	end process;


	process
		variable	WRITE_COUNT	: integer range 0 to 16;
		variable	END_FLAG	: boolean;
		variable	OUTPUT_LINE	: line;
	begin
		DMAREQ_I <= 'U';
		if (RESET /= '0') then
			wait until RESET = '0';
		end if;
		DMAREQ_I <= '0';
		wait until RESET = '1';
		while (true) loop
			wait until rising_edge(TRIGGER);
			END_FLAG := false;
			wait for 40 ns;
			while not END_FLAG loop
				DMAREQ_I <= '1';
				WRITE_COUNT := 0;
				while ((WRITE_COUNT < 16) and (not END_FLAG)) loop
					wait until falling_edge(WR_N);
					wait for 1 ns;
					if (EOT_N = '0') then
						END_FLAG := true;
					end if;
					wait until rising_edge(WR_N);
					write(OUTPUT_LINE, to_integer(signed(DATA)));
					writeline(OUTPUT_FILE, OUTPUT_LINE);
					WRITE_COUNT := WRITE_COUNT + 1;
				end loop;
				wait for 40 ns;
--------------------------------------------------
-- 				if (not END_FLAG) then            
-- 					wait for 40 ns - (now - TWL); 
-- 				else                              
-- 					wait for 40 ns - (now - TEL); 
-- 				end if;                           
--------------------------------------------------
				DMAREQ_I <= '0';
				wait for 500 ns;
			end loop;
		end loop;
    end process;
end architecture TESTBENCH;