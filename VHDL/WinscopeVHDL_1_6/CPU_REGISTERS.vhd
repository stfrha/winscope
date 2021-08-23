-------------------------------------------------------------------------------
-- File name    :  $Id: CPU_REGISTERS.vhd,v 1.9 2003/06/16 13:27:51 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: CPU_REGISTERS.vhd,v $
-- Revision 1.9  2003/06/16 13:27:51  frha
-- Fixed bug with SM_FREE, new version 1.6
--
-- Revision 1.8  2003/06/16 08:58:41  frha
-- Changed rev value to 1.6
--
-- Revision 1.7  2003/06/12 12:30:35  frha
-- Added CAL_PULSE of 1kHz and TRIGG_LED
--
-- Revision 1.6  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity TRIGG_LED_DRIVER is
    port (
		CLK			    : in std_logic;
		RESET			: in std_logic;
		DID_TRIGG		: in std_logic;
		TRIGG_LED		: out std_logic
    );
end TRIGG_LED_DRIVER;

architecture BEHAV of TRIGG_LED_DRIVER is
    signal LED_COUNT : unsigned (21 downto 0);
    signal SYNC_DID_TRIGG : std_logic;
    signal OLD_DID_TRIGG : std_logic;
begin
    process (CLK, RESET)
    begin
        if (RESET = '0') then 
            LED_COUNT <= (others => '0');
            TRIGG_LED <= '1';
			SYNC_DID_TRIGG <= '0';
			OLD_DID_TRIGG <= '0';
        elsif (rising_edge(CLK)) then
        	SYNC_DID_TRIGG <= DID_TRIGG;
            OLD_DID_TRIGG <= SYNC_DID_TRIGG;
            TRIGG_LED <= '1';                       -- Assume no trigg, LED off
            if (LED_COUNT /= 0) then
                LED_COUNT <= LED_COUNT - 1;
                TRIGG_LED <= '0';                   -- Ignite LED
            end if;
            if (SYNC_DID_TRIGG = '1' and OLD_DID_TRIGG = '0') then
                LED_COUNT <= to_unsigned(3000000, 22);
--                LED_COUNT <= to_unsigned(15, 22);
                TRIGG_LED <= '0';                   -- Ignite LED
            end if;
        end if;
    end process;
end architecture BEHAV;



            
         

    
    

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CLK_EDGE_TO_SMP_CLK_PULSE is
	port (
		CLK				: in std_logic;
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		I				: in std_logic;
		Q				: out std_logic
	);
end CLK_EDGE_TO_SMP_CLK_PULSE;

architecture BEHAV of CLK_EDGE_TO_SMP_CLK_PULSE is
	signal	ACTIVE_EDGE		: std_logic;
	signal	ACTIVE_EDGE_D	: std_logic;
	signal	WAIT_DEACTIVATE : std_logic;
	signal	EDGE_ACK		: std_logic;
	signal	EDGE_ACK_D		: std_logic;
	signal	Q_I				: std_logic;
	signal	I_OLD			: std_logic;
begin
	EDGE_FIND : process(CLK, RESET)
	begin
		if (RESET = '0') then
			ACTIVE_EDGE <= '0';
			WAIT_DEACTIVATE <= '0';
			EDGE_ACK_D <= '0';
			I_OLD <= '0';
		elsif (rising_edge(CLK)) then
			I_OLD <= I;
			EDGE_ACK_D <= EDGE_ACK;
			if (ACTIVE_EDGE = '0' and WAIT_DEACTIVATE = '0' and I = '1' and I_OLD = '0') then	-- Adding edge detection on I, fix for bug in 1.0
				ACTIVE_EDGE <= '1';
				WAIT_DEACTIVATE <= '1';
			elsif (EDGE_ACK_D = '1') then
				ACTIVE_EDGE <= '0';
			end if;
			if (I = '0') then
				WAIT_DEACTIVATE <= '0';
			end if;
		end if;
	end process;

	PULSE_GEN : process(SMP_CLK, RESET)
	begin
		if (RESET = '0') then
			EDGE_ACK <= '0';
			Q_I <= '0';
			ACTIVE_EDGE_D <= '0';
		elsif (rising_edge(SMP_CLK)) then
			ACTIVE_EDGE_D <= ACTIVE_EDGE;
			if (Q_I = '1') then
 		    	Q_I <= '0';
			elsif (ACTIVE_EDGE_D = '1' and EDGE_ACK = '0') then
				EDGE_ACK <= '1';
				Q_I <= '1';
			elsif (EDGE_ACK = '1' and ACTIVE_EDGE_D = '0') then
				EDGE_ACK <= '0';
			end if;
		end if;
	end process;

	Q <= Q_I;

end architecture BEHAV;







library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CPU_REGISTERS is
	port (
		CLK				: in std_logic;
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;

		ALE				: in std_logic;
		DATA			: inout std_logic_vector(7 downto 0);
		CPU_RD_N		: in std_logic;
		CPU_WR_N		: in std_logic;

		D12_CS_N_I		: out std_logic;

		PRBA_DATA		: in std_logic_vector(7 downto 0);
		PRBB_DATA		: in std_logic_vector(7 downto 0);
		PODP_DATA		: in std_logic_vector(7 downto 0);
		PODQ_DATA		: in std_logic_vector(7 downto 0);

		SWEEP_STATE		: in std_logic_vector(2 downto 0);
        DMA_TX_DONE		: in std_logic;
        SWEEP_DONE		: in std_logic;
		DID_TRIGG       : in std_logic;

		PRBA_TRG_LEVEL	: out std_logic_vector(7 downto 0);
		PRBB_TRG_LEVEL	: out std_logic_vector(7 downto 0);
		PRB_TRG_SOURCE	: out std_logic;
		PRB_TRG_SLOPE	: out std_logic;
		PODP_TRG_PTRN	: out std_logic_vector(7 downto 0);
		PODQ_TRG_PTRN	: out std_logic_vector(7 downto 0);
		PODP_PTRN_VALID	: out std_logic_vector(7 downto 0);
		PODQ_PTRN_VALID	: out std_logic_vector(7 downto 0);
		PODP_EDGE_SLOPE	: out std_logic_vector(7 downto 0);
		PODQ_EDGE_SLOPE	: out std_logic_vector(7 downto 0);
		PODP_EDGE_VALID	: out std_logic_vector(7 downto 0);
		PODQ_EDGE_VALID	: out std_logic_vector(7 downto 0);
		TRIGG_SETUP		: out std_logic_vector(7 downto 0);
		TRIGG_DELY		: out unsigned(16 downto 0);
		BUFFER_SIZE		: out unsigned(16 downto 0);

		PRBA_ACTIVE		: out std_logic;
		PRBB_ACTIVE		: out std_logic;
		PODP_ACTIVE		: out std_logic;
		PODQ_ACTIVE		: out std_logic;

		CLK_CHANGE		: out std_logic;
		CLK_TOGGLE		: out std_logic_vector(23 downto 0);
		CLK_RESET		: out std_logic_vector(23 downto 0);
		CLK_SELECT		: out std_logic;

        IRQ_N             : out std_logic;

		PRBA_GAIN		: out std_logic_vector(3 downto 0);
		PRBB_GAIN		: out std_logic_vector(3 downto 0);
		PRBA_AC			: out std_logic;
		PRBB_AC			: out std_logic;
		PRBA_GND		: out std_logic;
		PRBB_GND		: out std_logic;

		LEDS			: out std_logic_vector(7 downto 0);
		LEDS_ACT        : out std_logic;

		TRIGG_LED       : out std_logic;
		
		NEXT_STATE		: out std_logic;
		NEXT_STATE_L	: out std_logic;
		SWEEP_MODE		: out std_logic_vector(1 downto 0)
	);
end CPU_REGISTERS;

architecture BEHAV of CPU_REGISTERS is


	component CLK_EDGE_TO_SMP_CLK_PULSE is
		port (
			CLK				: in std_logic;
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			I				: in std_logic;
			Q				: out std_logic
		);
	end component CLK_EDGE_TO_SMP_CLK_PULSE;

    component TRIGG_LED_DRIVER is
        port (
    		CLK	    		: in std_logic;
    		RESET			: in std_logic;
    		DID_TRIGG		: in std_logic;
    		TRIGG_LED		: out std_logic
        );
    end component TRIGG_LED_DRIVER;

	signal	ADDRESS	 			: std_logic_vector(7 downto 0) := "HHHHHHHH";

	signal	PRBA_TRG_LEVEL_I	: std_logic_vector(7 downto 0);
	signal	PRBB_TRG_LEVEL_I	: std_logic_vector(7 downto 0);
	signal	PODP_TRG_PTRN_I		: std_logic_vector(7 downto 0);
	signal	PODQ_TRG_PTRN_I		: std_logic_vector(7 downto 0);
	signal	PODP_PTRN_VALID_I	: std_logic_vector(7 downto 0);
	signal	PODQ_PTRN_VALID_I	: std_logic_vector(7 downto 0);
	signal	PODP_EDGE_SLOPE_I	: std_logic_vector(7 downto 0);
	signal	PODQ_EDGE_SLOPE_I	: std_logic_vector(7 downto 0);
	signal	PODP_EDGE_VALID_I	: std_logic_vector(7 downto 0);
	signal	PODQ_EDGE_VALID_I	: std_logic_vector(7 downto 0);
	signal	TRIGG_SETUP_I		: std_logic_vector(7 downto 0);
	signal	TRIGG_DELY_I		: unsigned(16 downto 0);
	signal	BUFFER_SIZE_I		: unsigned(16 downto 0);
	signal	PRBA_ACTIVE_I		: std_logic;
	signal	PRBB_ACTIVE_I		: std_logic;
	signal	PODP_ACTIVE_I		: std_logic;
	signal	PODQ_ACTIVE_I		: std_logic;
	signal	NEXT_STATE_I		: std_logic;
	signal	LOCK_PRB_POD_I		: std_logic;
	signal  LOCK_PRB_POD_OLD	: std_logic;
	signal	SWEEP_MODE_I		: std_logic_vector(1 downto 0);

	signal	PRBA_DATA_I		    : std_logic_vector(7 downto 0);
	signal	PRBB_DATA_I		    : std_logic_vector(7 downto 0);
	signal	PODP_DATA_I		    : std_logic_vector(7 downto 0);
	signal	PODQ_DATA_I		    : std_logic_vector(7 downto 0);

	signal	CLK_TOGGLE_I		: std_logic_vector(23 downto 0);
	signal	CLK_RESET_I			: std_logic_vector(23 downto 0);
	signal	CLK_SELECT_I		: std_logic;
	signal	IRQ_EN_I    		: std_logic;

	signal	PRBA_GAIN_I			: std_logic_vector(3 downto 0);
	signal	PRBB_GAIN_I			: std_logic_vector(3 downto 0);
	signal	PRBA_AC_I			: std_logic;
	signal	PRBB_AC_I			: std_logic;
	signal	PRBA_GND_I			: std_logic;
	signal	PRBB_GND_I			: std_logic;

	signal	LEDS_I				: std_logic_vector(7 downto 0);

	signal 	DATA_D				: std_logic_vector(7 downto 0);
	signal  WR_N_D				: std_logic;

	signal  ID_L                : std_logic_vector(7 downto 0);
	signal  ID_H                : std_logic_vector(7 downto 0);
	signal  REV_L               : std_logic_vector(7 downto 0);
	signal  REV_H               : std_logic_vector(7 downto 0);

	signal	DATAX1_I	        : std_logic_vector(7 downto 0);
	signal	DATAX2_I	        : std_logic_vector(7 downto 0);
	signal	DATAX3_I	        : std_logic_vector(7 downto 0);
	signal	DATAX4_I	        : std_logic_vector(7 downto 0);
	signal	DATAX5_I	        : std_logic_vector(7 downto 0);
	signal	DATAX6_I	        : std_logic_vector(7 downto 0);
	signal	DATAX7_I	        : std_logic_vector(7 downto 0);
	signal	DATAX8_I	        : std_logic_vector(7 downto 0);

	signal  SWEEP_DONE_OLD  	: std_logic;
	signal  DMA_TX_DONE_OLD  	: std_logic;

	signal VCC                  : std_logic := '1';

	constant    ID_C          : std_logic_vector(7 downto 0) := x"55";
	constant    REV_C         : std_logic_vector(7 downto 0) := x"16";  -- 1.6
begin
    
    VCC <= '1';

	ALE_PROC : process(ALE, RESET, DATA)         -- SHould create a latch
	begin
		if (RESET = '0') then
			ADDRESS <= (others => '1');
		elsif ALE = '1' then
			ADDRESS <= DATA;
		end if;
	end process;

	D12_CS_N_I <= '0' when ADDRESS(7 downto 1) = "0000000" else '1';

    DATA_LOCK : process(CLK, RESET)
    begin
        if (RESET = '0') then
            DATAX1_I <= (others => '0');
            DATAX2_I <= (others => '0');
            DATAX3_I <= (others => '0');
            DATAX4_I <= (others => '0');
            DATAX5_I <= (others => '0');
            DATAX6_I <= (others => '0');
            DATAX7_I <= (others => '0');
            DATAX8_I <= (others => '0');
        elsif (rising_edge(CLK)) then
            case ADDRESS(3 downto 0) is
            when x"0" =>
                DATAX1_I <= TRIGG_SETUP_I;
                DATAX2_I <= std_logic_vector(TRIGG_DELY_I(7 downto 0));
                DATAX3_I <= "0000" & IRQ_EN_I & CLK_SELECT_I & SWEEP_MODE_I;
                DATAX4_I <= PRBA_DATA_I;
                DATAX5_I <= CLK_TOGGLE_I(7 downto 0);
                DATAX6_I <= PRBB_GAIN_I & PRBA_GAIN_I;
                DATAX7_I <= LEDS_I;
                DATAX8_I <= ID_C;
            when x"1" =>
                DATAX1_I <= PRBA_TRG_LEVEL_I;
                DATAX2_I <= std_logic_vector(TRIGG_DELY_I(15 downto 8));
                DATAX3_I <= SWEEP_STATE & "00" & DID_TRIGG & DMA_TX_DONE & SWEEP_DONE;
                DATAX4_I <= PRBB_DATA_I;
                DATAX5_I <= CLK_TOGGLE_I(15 downto 8);
                DATAX6_I <= PODQ_ACTIVE_I & PODP_ACTIVE_I & PRBB_ACTIVE_I & PRBA_ACTIVE_I & PRBB_GND_I & PRBB_AC_I &  PRBA_GND_I & PRBA_AC_I;
            when x"2" =>
                DATAX1_I <= PRBB_TRG_LEVEL_I;
                DATAX2_I <= "0000000" & TRIGG_DELY_I(16);
                DATAX4_I <= PODP_DATA_I;
                DATAX5_I <= CLK_TOGGLE_I(23 downto 16);
                DATAX8_I <= REV_C;
            when x"3" =>
                DATAX1_I <= PODP_TRG_PTRN_I;
                DATAX4_I <= PODQ_DATA_I;
            when x"4" =>
                DATAX1_I <= PODQ_TRG_PTRN_I;
                DATAX2_I <= std_logic_vector(BUFFER_SIZE_I(7 downto 0));
                DATAX5_I <= CLK_RESET_I(7 downto 0);
            when x"5" =>
                DATAX1_I <= PODP_PTRN_VALID_I;
                DATAX2_I <= std_logic_vector(BUFFER_SIZE_I(15 downto 8));
                DATAX5_I <= CLK_RESET_I(15 downto 8);
            when x"6" =>
                DATAX1_I <= PODQ_PTRN_VALID_I;
                DATAX2_I <= "0000000" & BUFFER_SIZE_I(16);
                DATAX5_I <= CLK_RESET_I(23 downto 16);
            when x"7" =>
                DATAX1_I <= PODP_EDGE_SLOPE_I;
            when x"8" =>
                DATAX1_I <= PODQ_EDGE_SLOPE_I;
            when x"9" =>
                DATAX1_I <= PODP_EDGE_VALID_I;
            when x"A" =>
                DATAX1_I <= PODQ_EDGE_VALID_I;
            when others =>
                null;
            end case;
        end if;
    end process;
    
    DATA <= DATAX1_I when ADDRESS(7 downto 4) = X"1" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX2_I when ADDRESS(7 downto 4) = X"2" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX3_I when ADDRESS(7 downto 4) = X"3" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX4_I when ADDRESS(7 downto 4) = X"4" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX5_I when ADDRESS(7 downto 4) = X"5" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX6_I when ADDRESS(7 downto 4) = X"6" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX7_I when ADDRESS(7 downto 4) = X"7" and (CPU_RD_N = '0') else "ZZZZZZZZ";            
    DATA <= DATAX8_I when ADDRESS(7 downto 4) = X"8" and (CPU_RD_N = '0') else "ZZZZZZZZ";            

	process(CLK, RESET)
	begin
		if (RESET = '0') then
			NEXT_STATE_I <= '0';
			LOCK_PRB_POD_I <= '0';
			SWEEP_MODE_I <= (others => '0');
			PRBA_TRG_LEVEL_I <= (others => '0');
			PRBB_TRG_LEVEL_I <= (others => '0');
			PODP_TRG_PTRN_I <= (others => '0');
			PODQ_TRG_PTRN_I <= (others => '0');
			PODP_PTRN_VALID_I <= (others => '0');
			PODQ_PTRN_VALID_I <= (others => '0');
			PODP_EDGE_SLOPE_I <= (others => '0');
			PODQ_EDGE_SLOPE_I <= (others => '0');
			PODP_EDGE_VALID_I <= (others => '0');
			PODQ_EDGE_VALID_I <= (others => '0');
			TRIGG_DELY_I <= to_unsigned(0, 17);
			BUFFER_SIZE_I <= to_unsigned(0, 17);
			TRIGG_SETUP_I <= (others => '0');
			PRBA_ACTIVE_I <= '1';						-- Fix for bug in 1.0
			PRBB_ACTIVE_I <= '1';
			PODP_ACTIVE_I <= '1';
			PODQ_ACTIVE_I <= '1';
			CLK_TOGGLE_I <= (others => '0');
			CLK_RESET_I <= (others => '0');
			CLK_SELECT_I <= '0';
			IRQ_EN_I <= '0';
			PRBA_GAIN_I <= (others => '0');
			PRBB_GAIN_I <= (others => '0');
			PRBA_AC_I <= '0';
			PRBA_GND_I <= '0';
			PRBB_AC_I <= '0';
			PRBB_GND_I <= '0';
			LEDS_I <= (others => '0');
			CLK_CHANGE <= '0';
			WR_N_D <= '1';
			DATA_D <= (others => '1');
            LEDS_ACT <= '0';
		elsif (rising_edge(CLK)) then
			DATA_D <= DATA;
			WR_N_D <= CPU_WR_N;
			CLK_CHANGE <= '0';														-- Reset to only generate one short pulse per write
			NEXT_STATE_I <= '0';													-- Reset to only generate one short pulse per write
			LOCK_PRB_POD_I <= '0';	    												-- Reset to only generate one short pulse per write
            LEDS_ACT <= '0';
			if (WR_N_D = '0') then
				case ADDRESS is
				when x"10" =>
                    TRIGG_SETUP_I <= DATA_D;
				when x"11" =>
					PRBA_TRG_LEVEL_I <= DATA_D;
				when x"12" =>
					PRBB_TRG_LEVEL_I <= DATA_D;
				when x"13" =>
					PODP_TRG_PTRN_I <= DATA_D;
				when x"14" =>
					PODQ_TRG_PTRN_I <= DATA_D;
				when x"15" =>
					PODP_PTRN_VALID_I <= DATA_D;
				when x"16" =>
					PODQ_PTRN_VALID_I <= DATA_D;
				when x"17" =>
					PODP_EDGE_SLOPE_I <= DATA_D;
				when x"18" =>
					PODQ_EDGE_SLOPE_I <= DATA_D;
				when x"19" =>
					PODP_EDGE_VALID_I <= DATA_D;
				when x"1A" =>
					PODQ_EDGE_VALID_I <= DATA_D;
				when x"20" =>
					TRIGG_DELY_I(7 downto 0) <= unsigned(DATA_D);
				when x"21" =>
					TRIGG_DELY_I(15 downto 8) <= unsigned(DATA_D);
				when x"22" =>
					TRIGG_DELY_I(16) <= DATA_D(0);
				when x"24" =>
					BUFFER_SIZE_I(7 downto 0) <= unsigned(DATA_D);
				when x"25" =>
					BUFFER_SIZE_I(15 downto 8) <= unsigned(DATA_D);
				when x"26" =>
					BUFFER_SIZE_I(16) <= DATA_D(0);
				when x"30" =>
					NEXT_STATE_I <= DATA_D(7);											-- Creates a short pulse on NEXT_STATE
                    LOCK_PRB_POD_I <= DATA_D(6);
                    IRQ_EN_I <= DATA_D(3);
					CLK_SELECT_I <= DATA_D(2);
					SWEEP_MODE_I <= DATA_D(1 downto 0);
				when x"50" =>
					CLK_TOGGLE_I(7 downto 0) <= DATA_D;
					CLK_CHANGE <= '1';													-- Created one short pulse on CLK_CHANGE
				when x"51" =>
					CLK_TOGGLE_I(15 downto 8) <= DATA_D;
					CLK_CHANGE <= '1';													-- Created one short pulse on CLK_CHANGE
				when x"52" =>
					CLK_TOGGLE_I(23 downto 16) <= DATA_D;
					CLK_CHANGE <= '1';													-- Created one short pulse on CLK_CHANGE
				when x"54" =>
					CLK_RESET_I(7 downto 0) <= DATA_D;
					CLK_CHANGE <= '1';													-- Created one short pulse on CLK_CHANGE
				when x"55" =>
					CLK_RESET_I(15 downto 8) <= DATA_D;
					CLK_CHANGE <= '1';													-- Created one short pulse on CLK_CHANGE
				when x"56" =>
					CLK_RESET_I(23 downto 16) <= DATA_D;
					CLK_CHANGE <= '1';													-- Created one short pulse on CLK_CHANGE
				when x"60" =>
					PRBA_GAIN_I <= DATA_D(3 downto 0);
					PRBB_GAIN_I <= DATA_D(7 downto 4);
				when x"61" =>
					PRBA_AC_I <= DATA_D(0);
					PRBA_GND_I <= DATA_D(1);
					PRBB_AC_I <= DATA_D(2);
					PRBB_GND_I <= DATA_D(3);
					PRBA_ACTIVE_I <= DATA_D(4);
					PRBB_ACTIVE_I <= DATA_D(5);
					PODP_ACTIVE_I <= DATA_D(6);
					PODQ_ACTIVE_I <= DATA_D(7);
				when x"70" =>
					LEDS_I <= DATA_D;
                    LEDS_ACT <= '1';
				when others =>
					NULL;
				end case;
			end if;
		end if;
	end process;

	process(CLK, RESET)
	begin
		if (RESET = '0') then
			LOCK_PRB_POD_OLD <= '0';
			PRBA_DATA_I <= (others => '1');
			PRBB_DATA_I <= (others => '1');
			PODP_DATA_I <= (others => '1');
			PODQ_DATA_I <= (others => '1');
		elsif (rising_edge(CLK)) then
			LOCK_PRB_POD_OLD <= LOCK_PRB_POD_I;
			if (LOCK_PRB_POD_I = '1' and LOCK_PRB_POD_OLD = '0') then
				PRBA_DATA_I <= PRBA_DATA;
				PRBB_DATA_I <= PRBB_DATA;
				PODP_DATA_I <= PODP_DATA;
				PODQ_DATA_I <= PODQ_DATA;
			end if;
        end if;
    end process;

	process(CLK, RESET)
	begin
		if (RESET = '0') then
		    SWEEP_DONE_OLD <= '0';
		    DMA_TX_DONE_OLD <= '0';
			IRQ_N <= '1';
		elsif (rising_edge(CLK)) then
		    SWEEP_DONE_OLD <= SWEEP_DONE;
		    DMA_TX_DONE_OLD <= DMA_TX_DONE;
            if (IRQ_EN_I = '1' and ((SWEEP_DONE = '1' and SWEEP_DONE_OLD = '0') or (DMA_TX_DONE = '1' and DMA_TX_DONE_OLD = '0'))) then
                IRQ_N <= '0';
            end if;
            if (ADDRESS = x"31" and CPU_RD_N = '0') then
                IRQ_N <= '1';
            end if;
        end if;
    end process;

	PRBA_TRG_LEVEL <= PRBA_TRG_LEVEL_I;
	PRBB_TRG_LEVEL <= PRBB_TRG_LEVEL_I;
	PRB_TRG_SOURCE <= TRIGG_SETUP_I(6);
	PRB_TRG_SLOPE <= TRIGG_SETUP_I(5);
	PODP_TRG_PTRN <= PODP_TRG_PTRN_I;
	PODQ_TRG_PTRN <= PODQ_TRG_PTRN_I;
	PODP_PTRN_VALID <= PODP_PTRN_VALID_I;
	PODQ_PTRN_VALID <= PODQ_PTRN_VALID_I;
	PODP_EDGE_SLOPE <= PODP_EDGE_SLOPE_I;
	PODQ_EDGE_SLOPE <= PODQ_EDGE_SLOPE_I;
	PODP_EDGE_VALID <= PODP_EDGE_VALID_I;
	PODQ_EDGE_VALID <= PODQ_EDGE_VALID_I;
	TRIGG_SETUP <= TRIGG_SETUP_I;
	TRIGG_DELY <= TRIGG_DELY_I;
	BUFFER_SIZE <= BUFFER_SIZE_I;

	PRBA_ACTIVE <= PRBA_ACTIVE_I;
	PRBB_ACTIVE <= PRBB_ACTIVE_I;
	PODP_ACTIVE <= PODP_ACTIVE_I;
	PODQ_ACTIVE <= PODQ_ACTIVE_I;

	CLK_TOGGLE <= CLK_TOGGLE_I;
	CLK_RESET <= CLK_RESET_I;
	CLK_SELECT <= CLK_SELECT_I;

	PRBA_GAIN <= PRBA_GAIN_I;
	PRBB_GAIN <= PRBB_GAIN_I;
	PRBA_AC <= PRBA_AC_I;
	PRBB_AC <= PRBB_AC_I;
	PRBA_GND <= PRBA_GND_I;
	PRBB_GND <= PRBB_GND_I;

	LEDS <= LEDS_I;
	
	SWEEP_MODE <= SWEEP_MODE_I;

	NSSYN : CLK_EDGE_TO_SMP_CLK_PULSE port map (
		CLK => CLK,
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		I => NEXT_STATE_I,
		Q => NEXT_STATE
	);

    TL1 :  TRIGG_LED_DRIVER port map (
   		CLK => CLK,
   		RESET => RESET,
   		DID_TRIGG => DID_TRIGG,
   		TRIGG_LED => TRIGG_LED
    );
    NEXT_STATE_L <= NEXT_STATE_I;       -- CLK ref version of NEXT_STATE

end architecture BEHAV;






