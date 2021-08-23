-------------------------------------------------------------------------------
-- File name    :  $Id: Trigger.vhd,v 1.5 2003/06/16 08:52:35 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: Trigger.vhd,v $
-- Revision 1.5  2003/06/16 08:52:35  frha
-- Fixed bug in SM_FREE, State PRE_FILL is now entered on NEXT_STATE instead of TRIGGED
--
-- Revision 1.4  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity PRB_TRIGG_C is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		PRBA_IN			: in std_logic_vector(7 downto 0);
		PRBB_IN			: in std_logic_vector(7 downto 0);
		PRBA_TRG_LEVEL	: in std_logic_vector(7 downto 0);
		PRBB_TRG_LEVEL	: in std_logic_vector(7 downto 0);
		PRB_TRG_SOURCE	: in std_logic;				-- PRB_TRG_SOURCE = '0' means probe A is trigg source
		PRB_TRG_SLOPE	: in std_logic;				-- PRB_TRG_SLOPE = '0' means negative slope trigger
		PRB_TRIGG		: out std_logic
	);
end PRB_TRIGG_C;

architecture BEHAV of PRB_TRIGG_C is
	signal	PRBA_ABOVE	 	: std_logic;
	signal	PRBA_ABOVE_D 	: std_logic;
	signal	PRBB_ABOVE	 	: std_logic;
	signal	PRBB_ABOVE_D 	: std_logic;
begin
	
	PRBA_ABOVE <= '1' when unsigned(PRBA_IN) >= unsigned(PRBA_TRG_LEVEL) else '0';
	PRBB_ABOVE <= '1' when unsigned(PRBB_IN) >= unsigned(PRBB_TRG_LEVEL) else '0';
	
	process(SMP_CLK, RESET)
	begin
		if (RESET = '0') then
			PRB_TRIGG <= '0';
			PRBA_ABOVE_D <= '0';
			PRBB_ABOVE_D <= '0';
		elsif (rising_edge(SMP_CLK)) then
			PRB_TRIGG <= '0';
			PRBA_ABOVE_D <= PRBA_ABOVE;
			PRBB_ABOVE_D <= PRBB_ABOVE;
			if (PRB_TRG_SOURCE = '0') then
				if ((PRB_TRG_SLOPE = '0' and PRBA_ABOVE = '0' and PRBA_ABOVE_D = '1') or 
				   (PRB_TRG_SLOPE = '1' and PRBA_ABOVE = '1' and PRBA_ABOVE_D = '0')) then
					PRB_TRIGG <= '1';
				end if;
			else
				if ((PRB_TRG_SLOPE = '0' and PRBB_ABOVE = '0' and PRBB_ABOVE_D = '1') or 
				   (PRB_TRG_SLOPE = '1' and PRBB_ABOVE = '1' and PRBB_ABOVE_D = '0')) then
					PRB_TRIGG <= '1';
				end if;
			end if;
		end if;
	end process;
end architecture BEHAV;

				
			



library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity POD_TRIGG is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		PODP_IN			: in std_logic_vector(7 downto 0);
		PODQ_IN			: in std_logic_vector(7 downto 0);
		PODP_TRG_PTRN	: in std_logic_vector(7 downto 0);
		PODQ_TRG_PTRN	: in std_logic_vector(7 downto 0);
		PODP_PTRN_VALID	: in std_logic_vector(7 downto 0);
		PODQ_PTRN_VALID	: in std_logic_vector(7 downto 0);
		PODP_EDGE_SLOPE	: in std_logic_vector(7 downto 0);
		PODQ_EDGE_SLOPE	: in std_logic_vector(7 downto 0);
		PODP_EDGE_VALID	: in std_logic_vector(7 downto 0);
		PODQ_EDGE_VALID	: in std_logic_vector(7 downto 0);
		POD_PTRN		: out std_logic;
		POD_EDGE		: out std_logic
	);
end POD_TRIGG;

architecture BEHAV of POD_TRIGG is
	signal PODP_IN_D	: std_logic_vector(7 downto 0);
	signal PODQ_IN_D	: std_logic_vector(7 downto 0);
begin
	process(RESET, SMP_CLK)
	begin
		if (RESET = '0') then
			PODP_IN_D <= (others => '0');
			PODQ_IN_D <= (others => '0');
			POD_PTRN <= '0';
			POD_EDGE <= '0';
		elsif (rising_edge(SMP_CLK)) then
			PODP_IN_D <= PODP_IN;
			PODQ_IN_D <= PODQ_IN;
--			POD_PTRN <= '1';				-- Assume pattern active, will be reset at first found pattern error
			POD_EDGE <= '0';
				   				   
			if (((PODP_IN and PODP_PTRN_VALID) = (PODP_TRG_PTRN and PODP_PTRN_VALID)) and
				((PODQ_IN and PODQ_PTRN_VALID) = (PODQ_TRG_PTRN and PODQ_PTRN_VALID))) then
				POD_PTRN <= '1';
			else
				POD_PTRN <= '0';
			end if;

				   
			for I in 0 to 7 loop
-------------------------------------------------------------**
-- 				if (PODP_PTRN_VALID(I) = '1') then           **
-- 					if (PODP_IN(I) /= PODP_TRG_PTRN(I)) then **
-- 						POD_PTRN <= '0';                     **
-- 					end if;                                  **
-- 				end if;                                      **
--                                                           **
-- 				if (PODQ_PTRN_VALID(I) = '1') then           **
-- 					if (PODQ_IN(I) /= PODQ_TRG_PTRN(I)) then **
-- 						POD_PTRN <= '0';                     **
-- 					end if;                                  **
-- 				end if;                                      **
-------------------------------------------------------------*/

				if (PODP_EDGE_VALID(I) = '1') then
					if ((PODP_EDGE_SLOPE(I) = '0' and PODP_IN(I) = '0' and PODP_IN_D(I) = '1') or
					   	(PODP_EDGE_SLOPE(I) = '1' and PODP_IN(I) = '1' and PODP_IN_D(I) = '0')) then
						POD_EDGE <= '1';
					end if;
				end if;

				if (PODQ_EDGE_VALID(I) = '1') then
					if ((PODQ_EDGE_SLOPE(I) = '0' and PODQ_IN(I) = '0' and PODQ_IN_D(I) = '1') or
					   	(PODQ_EDGE_SLOPE(I) = '1' and PODQ_IN(I) = '1' and PODQ_IN_D(I) = '0')) then
						POD_EDGE <= '1';
					end if;
				end if;
			end loop;
		end if;
	end process;
end architecture BEHAV;








library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity TRIGG_SELECTION is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		PRB_TRIGG		: in std_logic;
		POD_PTRN		: in std_logic;
		POD_EDGE		: in std_logic;
		EXT_TRIGG		: in std_logic;
		TRIGG_SETUP		: in std_logic_vector(7 downto 0);
		TRIGG			: out std_logic
	);
end TRIGG_SELECTION;

architecture BEHAV of TRIGG_SELECTION is
	signal	EXT_TRIGG_I	: std_logic;
	signal	EXT_TRIGG_D	: std_logic;
begin
	process(RESET, SMP_CLK)
	begin
		if (RESET = '0') then
			TRIGG <= '0';
			EXT_TRIGG_I <= '0';
			EXT_TRIGG_D <= '0';
		elsif (rising_edge(SMP_CLK)) then
			EXT_TRIGG_D <= EXT_TRIGG;
			if ((TRIGG_SETUP(7) = '0' and EXT_TRIGG = '0' and EXT_TRIGG_D = '1') or
				(TRIGG_SETUP(7) = '1' and EXT_TRIGG = '1' and EXT_TRIGG_D = '0')) then
				EXT_TRIGG_I <= '1';
			else
				EXT_TRIGG_I <= '0';
			end if;

			TRIGG <= '0';

			if (TRIGG_SETUP(0) = '1') then 							-- Trigg always
				TRIGG <= '1';
			else
				case TRIGG_SETUP(4 downto 1) is
				when "0010" =>										-- Pod pattern
					TRIGG <= POD_PTRN;
				when "0011" =>										-- Pod pattern and probe edge
					TRIGG <= POD_PTRN and PRB_TRIGG;
				when "0110" =>										-- Pod pattern and pod edge
					TRIGG <= POD_PTRN and POD_EDGE;
				when "1010" =>										-- Pod pattern and external edge
					TRIGG <= POD_PTRN and EXT_TRIGG_I;
				when "0001" =>										-- Probe edge
					TRIGG <= PRB_TRIGG;
				when "0100" =>										-- Pod edge
					TRIGG <= POD_EDGE;
				when "1000" =>										-- External edge
					TRIGG <= EXT_TRIGG_I;
				when others =>
					TRIGG <= '0';
				end case;
			end if;
		end if;
	end process;
end architecture BEHAV;





library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity SWEEP_COUNTER is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		START_PRE_COUNT	: in std_logic;
		START_TRG_COUNT	: in std_logic;
		TRIGG_DELY		: in unsigned(16 downto 0);
		BUFFER_SIZE		: in unsigned(16 downto 0);
		COUNTING		: out std_logic
	);
end SWEEP_COUNTER;

architecture BEHAV of SWEEP_COUNTER is
	type 	CNT_STATE_TYPE is (PRE, TRG);
	signal	CNT_STATE	: CNT_STATE_TYPE;
	signal	COUNT	: unsigned(16 downto 0);
begin
	process(RESET, SMP_CLK)
	begin
		if (RESET = '0') then
			COUNT <= (others => '0');
			CNT_STATE <= PRE;
			COUNTING <= '0';
		elsif (rising_edge(SMP_CLK)) then
			case CNT_STATE is
			when PRE =>
				if (START_PRE_COUNT = '1') then
					COUNT <= (others => '0');
					COUNTING <= '1';
				else
					if (COUNT < TRIGG_DELY) then
						COUNT <= COUNT + 1;
					else
						COUNTING <= '0';
					end if;
				end if;
				if (START_TRG_COUNT = '1') then
					CNT_STATE <= TRG;
					COUNTING <= '1';
				end if;
			when TRG =>
				if (COUNT < BUFFER_SIZE) then
					COUNT <= COUNT + 1;
				else
					COUNTING <= '0';
					CNT_STATE <= PRE;
				end if;
			end case;
		end if;
	end process;
end architecture BEHAV;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity TRIGG_FSM is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;
		COUNTING		: in std_logic;
		TRIGG			: in std_logic;
		NEXT_STATE		: in std_logic;
		SWEEP_MODE		: in std_logic_vector(1 downto 0);
		START_PRE_COUNT	: out std_logic;
		START_TRG_COUNT	: out std_logic;
		SAVE_LOAD_N		: out std_logic;
        DID_TRIGG       : out std_logic;
		SWEEP_STATE		: out std_logic_vector(2 downto 0)
	);
end TRIGG_FSM;

architecture BEHAV of TRIGG_FSM is
	type	SC_STATE_TYPE is (IDLE, D_PRE_FILL, PRE_FILL, ARMED, D_TRIGGED, TRIGGED, FULL);
	signal	STATE			: SC_STATE_TYPE;
	signal 	SAVE_LOAD_N_I		: std_logic;
begin
	process(RESET, SMP_CLK)
	begin
		if (RESET = '0') then
			STATE <= IDLE;
			SAVE_LOAD_N_I <= '1';
			SWEEP_STATE(1 downto 0) <= "00";
			START_PRE_COUNT <= '0';
			START_TRG_COUNT <= '0';
            DID_TRIGG <= '0';
		elsif (rising_edge(SMP_CLK)) then
		    
            if (NEXT_STATE = '1') then
                DID_TRIGG <= '0';
            else
                if (TRIGG = '1') then
                    DID_TRIGG <= '1';
                end if;
            end if;

			case STATE is
			when IDLE =>
				if (NEXT_STATE = '1') then
					START_PRE_COUNT <= '1';
					SWEEP_STATE(1 downto 0) <= "01";
					STATE <= D_PRE_FILL;
				end if;
			when D_PRE_FILL =>								-- Allows one clock before COUNTING is reset
				START_PRE_COUNT <= '0';						-- Only one short pulse
				STATE <= PRE_FILL;
			when PRE_FILL =>
				if (COUNTING = '0') then
					SWEEP_STATE(1 downto 0) <= "10";
					STATE <= ARMED;
				end if;
				if (NEXT_STATE = '1') then
					if (SWEEP_MODE = "10") then				-- SM_SINGLE = 10
						SWEEP_STATE(1 downto 0) <= "00";
						STATE <= IDLE;
					else                        			-- SM_NORMAL = 00 or SM_FREE = 01
						START_PRE_COUNT <= '1';
						SWEEP_STATE(1 downto 0) <= "01";
						STATE <= D_PRE_FILL;
					end if;
				end if;
			when ARMED =>
				if (TRIGG = '1') or (SWEEP_MODE = "01") then -- If Auto, ignore trigger
					START_TRG_COUNT <= '1';
					SWEEP_STATE(1 downto 0) <= "11";
					STATE <= D_TRIGGED;
				end if;
				if (NEXT_STATE = '1') then
					if (SWEEP_MODE = "10") then				-- SM_SINGLE = 10
						SWEEP_STATE(1 downto 0) <= "00";
						STATE <= IDLE;
					else                        			-- SM_NORMAL = 00 or SM_FREE = 01
						START_PRE_COUNT <= '1';
						SWEEP_STATE(1 downto 0) <= "01";
						STATE <= D_PRE_FILL;
					end if;
				end if;
			when D_TRIGGED =>								-- Allows one clock before COUNTING is reset
				START_TRG_COUNT <= '0';						-- Only one short pulse
				STATE <= TRIGGED;
			when TRIGGED =>
				if (COUNTING = '0') then
					SAVE_LOAD_N_I <= '0';
					SWEEP_STATE(1 downto 0) <= "00";
					STATE <= FULL;
				end if;
			when FULL =>
				if (NEXT_STATE = '1') then
					SAVE_LOAD_N_I <= '1';
					if (SWEEP_MODE = "10") then				-- SM_SINGLE = 10
						SWEEP_STATE(1 downto 0) <= "00";
						STATE <= IDLE;
					else                        			-- SM_NORMAL = 00 or SM_FREE = 01
						START_PRE_COUNT <= '1';             -- Free also goes here
						SWEEP_STATE(1 downto 0) <= "01";
						STATE <= D_PRE_FILL;
					end if;
				end if;
			end case;
		end if;
	end process;

	SWEEP_STATE(2) <= not SAVE_LOAD_N_I;
	SAVE_LOAD_N <= SAVE_LOAD_N_I;

end architecture BEHAV;









library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity TRIGGER is
	port (
		SMP_CLK			: in std_logic;
		RESET			: in std_logic;

		PRBA_IN			: in std_logic_vector(7 downto 0);
		PRBB_IN			: in std_logic_vector(7 downto 0);
		PRBA_TRG_LEVEL	: in std_logic_vector(7 downto 0);
		PRBB_TRG_LEVEL	: in std_logic_vector(7 downto 0);
		PRB_TRG_SOURCE	: in std_logic;
		PRB_TRG_SLOPE	: in std_logic;

		PODP_IN			: in std_logic_vector(7 downto 0);
		PODQ_IN			: in std_logic_vector(7 downto 0);
		PODP_TRG_PTRN	: in std_logic_vector(7 downto 0);
		PODQ_TRG_PTRN	: in std_logic_vector(7 downto 0);
		PODP_PTRN_VALID	: in std_logic_vector(7 downto 0);
		PODQ_PTRN_VALID	: in std_logic_vector(7 downto 0);
		PODP_EDGE_SLOPE	: in std_logic_vector(7 downto 0);
		PODQ_EDGE_SLOPE	: in std_logic_vector(7 downto 0);
		PODP_EDGE_VALID	: in std_logic_vector(7 downto 0);
		PODQ_EDGE_VALID	: in std_logic_vector(7 downto 0);

		EXT_TRIGG		: in std_logic;
		TRIGG_SETUP		: in std_logic_vector(7 downto 0);

		TRIGG_DELY		: in unsigned(16 downto 0);
		BUFFER_SIZE		: in unsigned(16 downto 0);

		NEXT_STATE		: in std_logic;
		SWEEP_MODE		: in std_logic_vector(1 downto 0);
		SAVE_LOAD_N		: out std_logic;
		DID_TRIGG       : out std_logic;
		SWEEP_STATE		: out std_logic_vector(2 downto 0)

	);
end TRIGGER;

architecture STRUCT of TRIGGER is
	component PRB_TRIGG_C is
		port (
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			PRBA_IN			: in std_logic_vector(7 downto 0);
			PRBB_IN			: in std_logic_vector(7 downto 0);
			PRBA_TRG_LEVEL	: in std_logic_vector(7 downto 0);
			PRBB_TRG_LEVEL	: in std_logic_vector(7 downto 0);
			PRB_TRG_SOURCE	: in std_logic;				-- PRB_TRG_SOURCE = '0' means probe A is trigg source
			PRB_TRG_SLOPE	: in std_logic;				-- PRB_TRG_SLOPE = '0' means negative slope trigger
			PRB_TRIGG		: out std_logic
		);
	end component PRB_TRIGG_C;

	component POD_TRIGG is
		port (
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			PODP_IN			: in std_logic_vector(7 downto 0);
			PODQ_IN			: in std_logic_vector(7 downto 0);
			PODP_TRG_PTRN	: in std_logic_vector(7 downto 0);
			PODQ_TRG_PTRN	: in std_logic_vector(7 downto 0);
			PODP_PTRN_VALID	: in std_logic_vector(7 downto 0);
			PODQ_PTRN_VALID	: in std_logic_vector(7 downto 0);
			PODP_EDGE_SLOPE	: in std_logic_vector(7 downto 0);
			PODQ_EDGE_SLOPE	: in std_logic_vector(7 downto 0);
			PODP_EDGE_VALID	: in std_logic_vector(7 downto 0);
			PODQ_EDGE_VALID	: in std_logic_vector(7 downto 0);
			POD_PTRN		: out std_logic;
			POD_EDGE		: out std_logic
		);
	end component POD_TRIGG;

	component TRIGG_SELECTION is
		port (
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			PRB_TRIGG		: in std_logic;
			POD_PTRN		: in std_logic;
			POD_EDGE		: in std_logic;
			EXT_TRIGG		: in std_logic;
			TRIGG_SETUP		: in std_logic_vector(7 downto 0);
			TRIGG			: out std_logic
		);
	end component TRIGG_SELECTION;

	component SWEEP_COUNTER is
		port (
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			START_PRE_COUNT	: in std_logic;
			START_TRG_COUNT	: in std_logic;
			TRIGG_DELY		: in unsigned(16 downto 0);
			BUFFER_SIZE		: in unsigned(16 downto 0);
			COUNTING		: out std_logic
		);
	end component SWEEP_COUNTER;

	component TRIGG_FSM is
		port (
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			COUNTING		: in std_logic;
			TRIGG			: in std_logic;
			NEXT_STATE		: in std_logic;
			SWEEP_MODE		: in std_logic_vector(1 downto 0);
			START_PRE_COUNT	: out std_logic;
			START_TRG_COUNT	: out std_logic;
			SAVE_LOAD_N		: out std_logic;
            DID_TRIGG       : out std_logic;
			SWEEP_STATE		: out std_logic_vector(2 downto 0)
		);
	end component TRIGG_FSM;

	signal	PRB_TRIGG		: std_logic;
	signal	POD_PTRN		: std_logic;
	signal	POD_EDGE		: std_logic;
	signal	TRIGG			: std_logic;
	signal	START_PRE_COUNT	: std_logic;
	signal	START_TRG_COUNT	: std_logic;
	signal	COUNTING		: std_logic;

begin
	PRT : PRB_TRIGG_C port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		PRBA_IN => PRBA_IN,
		PRBB_IN => PRBB_IN,
		PRBA_TRG_LEVEL => PRBA_TRG_LEVEL,
		PRBB_TRG_LEVEL => PRBB_TRG_LEVEL,
		PRB_TRG_SOURCE => PRB_TRG_SOURCE,
		PRB_TRG_SLOPE => PRB_TRG_SLOPE,
		PRB_TRIGG => PRB_TRIGG
	);

	PDT : POD_TRIGG port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		PODP_IN => PODP_IN,
		PODQ_IN => PODQ_IN,
		PODP_TRG_PTRN => PODP_TRG_PTRN,
		PODQ_TRG_PTRN => PODQ_TRG_PTRN,
		PODP_PTRN_VALID	=> PODP_PTRN_VALID,
		PODQ_PTRN_VALID	=> PODQ_PTRN_VALID,
		PODP_EDGE_SLOPE	=> PODP_EDGE_SLOPE,
		PODQ_EDGE_SLOPE	=> PODQ_EDGE_SLOPE,
		PODP_EDGE_VALID	=> PODP_EDGE_VALID,
		PODQ_EDGE_VALID	=> PODQ_EDGE_VALID,
		POD_PTRN => POD_PTRN,
		POD_EDGE => POD_EDGE
	);
		
	TRS : TRIGG_SELECTION port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		PRB_TRIGG => PRB_TRIGG,
		POD_PTRN => POD_PTRN,
		POD_EDGE => POD_EDGE,
		EXT_TRIGG => EXT_TRIGG,
		TRIGG_SETUP	=> TRIGG_SETUP,
		TRIGG => TRIGG
	);

	SWC : SWEEP_COUNTER port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		START_PRE_COUNT => START_PRE_COUNT,
		START_TRG_COUNT	=> START_TRG_COUNT,
		TRIGG_DELY => TRIGG_DELY,
		BUFFER_SIZE => BUFFER_SIZE,
		COUNTING => COUNTING
	);

	TFSM : TRIGG_FSM port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		COUNTING => COUNTING,
		TRIGG => TRIGG,
		NEXT_STATE => NEXT_STATE,
		SWEEP_MODE => SWEEP_MODE,
		START_PRE_COUNT => START_PRE_COUNT,
		START_TRG_COUNT	=> START_TRG_COUNT,
		SAVE_LOAD_N => SAVE_LOAD_N,
        DID_TRIGG => DID_TRIGG,
		SWEEP_STATE => SWEEP_STATE
	);
end architecture STRUCT;


