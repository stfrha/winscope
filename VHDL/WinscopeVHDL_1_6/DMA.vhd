-------------------------------------------------------------------------------
-- File name    :  $Id: DMA.vhd,v 1.5 2003/06/12 09:09:07 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: DMA.vhd,v $
-- Revision 1.5  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity DMA_CONTROLER is
	port (
		CLK				: in std_logic;
		RESET			: in std_logic;

		CPU_RD_N		: in std_logic;
		CPU_WR_N		: in std_logic;
		D12_CS_N_I		: in std_logic;

		BUFFER_SIZE		: in unsigned(16 downto 0);
		PRBA_ACTIVE		: in std_logic;
		PRBB_ACTIVE		: in std_logic;
		PODP_ACTIVE		: in std_logic;
		PODQ_ACTIVE		: in std_logic;
		PRBA_IN         : in std_logic_vector(7 downto 0);
		PRBB_IN         : in std_logic_vector(7 downto 0);
		PODP_IN         : in std_logic_vector(7 downto 0);
		PODQ_IN         : in std_logic_vector(7 downto 0);

        SAVE_LOAD_N     : in std_logic;

        DMAREQ          : in std_logic;

		RD_N			: out std_logic;
		WR_N			: out std_logic;
		D12_CS_N		: out std_logic;

		DMACK_N         : out std_logic;
        EOT_N           : out std_logic;

		DMA_TX_DONE		: out std_logic;

		DATA            : inout std_logic_vector(7 downto 0);
		LOAD_INC_PULSE  : out std_logic

	);
end DMA_CONTROLER;

architecture BEHAV of DMA_CONTROLER is
	type	DMC_STATE_TYPE is (IDLE, NEXT_ADR, INC_ADR, CHK_DMAREQ1, CHK_DMAREQ2, SETUP_DATA, WAITS, WRITE, DATA_HOLD, DMAREQ_LOW, LAST_DMAREQ_LOW);
	type	PRB_POD_TYPE is (PRBA, PRBB, PODP, PODQ, NONE);

    signal  STATE			: DMC_STATE_TYPE;
	signal	CURR_PRB_POD	: PRB_POD_TYPE;
	signal	NEXT_PRB_POD	: PRB_POD_TYPE;
    signal  HOLD_COUNTER    : integer range 0 to 13;
    signal  DMA_COUNTER     : integer range 0 to 131072;
	signal  DMA_CNT_ONE     : std_logic;
    signal  WR_N_I          : std_logic;
    signal  DMAREQ_SYNCED   : std_logic;
    signal  PRBA_LATCHED    : std_logic_vector(7 downto 0);
    signal  PRBB_LATCHED    : std_logic_vector(7 downto 0);
    signal  PODP_LATCHED    : std_logic_vector(7 downto 0);
    signal  PODQ_LATCHED    : std_logic_vector(7 downto 0);
    signal  DMA_DATA        : std_logic_vector(7 downto 0);
    signal  SAVE_LOAD_SYN   : std_logic;

begin
    SYNC : process(CLK, RESET)
    begin
        if (RESET = '0') then
            DMAREQ_SYNCED <= '0';
			SAVE_LOAD_SYN <= '0';
        elsif rising_edge(CLK) then
            DMAREQ_SYNCED <= DMAREQ;
			SAVE_LOAD_SYN <= SAVE_LOAD_N;
        end if;
    end process;

    FSM : process(CLK, RESET)
		variable	TMP_CURR	: PRB_POD_TYPE;
    begin
        if (RESET = '0') then
            STATE <= IDLE;
            DMACK_N <= '1';
			CURR_PRB_POD <= NONE;
			NEXT_PRB_POD <= NONE;
            DMA_COUNTER <= 0;
            DMA_CNT_ONE <= '0';
            WR_N_I <= '1';
            EOT_N <= 'Z';
            LOAD_INC_PULSE <= '0';
            DMA_TX_DONE <= '0';
            DMA_DATA <= (others => '0');
            PRBA_LATCHED <= (others => '0');
            PRBB_LATCHED <= (others => '0');
            PODP_LATCHED <= (others => '0');
            PODQ_LATCHED <= (others => '0');
        elsif (rising_edge(CLK)) then
            if (SAVE_LOAD_SYN = '1') then
                STATE <= IDLE;
                DMACK_N <= '1';
                DMA_CNT_ONE <= '0';
                WR_N_I <= '1';
                EOT_N <= 'Z';
                LOAD_INC_PULSE <= '0';
                DMA_TX_DONE <= '0';
    			CURR_PRB_POD <= NONE;
    			NEXT_PRB_POD <= NONE;
            else
                case STATE is
                when IDLE =>
                    DMA_COUNTER <= to_integer(BUFFER_SIZE);
                    if (DMAREQ_SYNCED = '1') then
                        STATE <= NEXT_ADR;
                        DMACK_N <= '0';
                        DMA_TX_DONE <= '0';
					end if;
        		when NEXT_ADR =>
					STATE <= INC_ADR;
                    PRBA_LATCHED <= PRBA_IN;
                    PRBB_LATCHED <= PRBB_IN;
                    PODP_LATCHED <= PODP_IN;
                    PODQ_LATCHED <= PODQ_IN;
                    LOAD_INC_PULSE <= '1';
                when INC_ADR =>
    				STATE <= CHK_DMAREQ1;
                    LOAD_INC_PULSE <= '0';
                    if (DMA_COUNTER = 1) then 
                        DMA_CNT_ONE <= '1';
                    else 
                        DMA_CNT_ONE <= '0';
                    end if;
--					GET_INITIAL_PRB_POD();
					if (PRBA_ACTIVE = '1') then
						CURR_PRB_POD <= PRBA;
						TMP_CURR := PRBA;
					elsif (PRBB_ACTIVE = '1') then
						CURR_PRB_POD <= PRBB;
						TMP_CURR := PRBB;
					elsif (PODP_ACTIVE = '1') then
						CURR_PRB_POD <= PODP;
						TMP_CURR := PODP;
					else
						CURR_PRB_POD <= PODQ;
						TMP_CURR := PODQ;
					end if;
					
					case (TMP_CURR) is
					when PRBA =>
						if (PRBB_ACTIVE = '1') then
							NEXT_PRB_POD <= PRBB;
						elsif (PODP_ACTIVE = '1') then
							NEXT_PRB_POD <= PODP;
						elsif (PODQ_ACTIVE = '1') then
							NEXT_PRB_POD <= PODQ;
						else
							NEXT_PRB_POD <= NONE;
						end if;
					when PRBB =>
						if (PODP_ACTIVE = '1') then
							NEXT_PRB_POD <= PODP;
						elsif (PODQ_ACTIVE = '1') then
							NEXT_PRB_POD <= PODQ;
						else
							NEXT_PRB_POD <= NONE;
						end if;
					when PODP =>
						if (PODQ_ACTIVE = '1') then
							NEXT_PRB_POD <= PODQ;
						else
							NEXT_PRB_POD <= NONE;
						end if;
					when PODQ =>
						NEXT_PRB_POD <= NONE;
					when NONE =>
						null;
					end case;
-- END GET_INITAL_PRB_POD()
				when CHK_DMAREQ1 =>
					if (DMAREQ_SYNCED = '0') then
						STATE <= DMAREQ_LOW;
					else
						STATE <= CHK_DMAREQ2;
					end if;
				when CHK_DMAREQ2 =>
					if (DMAREQ_SYNCED = '0') then
						STATE <= DMAREQ_LOW;
					else
						STATE <= SETUP_DATA;
					end if;
        		when SETUP_DATA =>
   					STATE <= WAITS;
					case CURR_PRB_POD is
					when PRBA =>
						DMA_DATA <= PRBA_LATCHED;
					when PRBB =>
						DMA_DATA <= PRBB_LATCHED;
					when PODP =>
						DMA_DATA <= PODP_LATCHED;
					when PODQ =>
						DMA_DATA <= PODQ_LATCHED;
					when NONE =>
						null;
					end case;
                when WAITS =>
    				if (HOLD_COUNTER = 0) then
    					STATE <= WRITE;
    					WR_N_I <= '0';
    					if ((DMA_CNT_ONE = '1') and (NEXT_PRB_POD = NONE)) then			-- if ((DMA_COUNTER = 1) and (NEXT_PRB_P0D = NONE)) then
    						EOT_N <= '0';
    					end if;
    				end if;
    			when WRITE =>
    				STATE <= DATA_HOLD;
    				WR_N_I <= '1';
    				EOT_N <= 'Z';
    --				GET_NEXT_PRB_POD();
    				CURR_PRB_POD <= NEXT_PRB_POD;
    				if (NEXT_PRB_POD /= NONE) then
    					case (NEXT_PRB_POD) is
    					when PRBA =>
    						if (PRBB_ACTIVE = '1') then
    							NEXT_PRB_POD <= PRBB;
    						elsif (PODP_ACTIVE = '1') then
    							NEXT_PRB_POD <= PODP;
    						elsif (PODQ_ACTIVE = '1') then
    							NEXT_PRB_POD <= PODQ;
    						else
    							NEXT_PRB_POD <= NONE;
    						end if;
    					when PRBB =>
    						if (PODP_ACTIVE = '1') then
    							NEXT_PRB_POD <= PODP;
    						elsif (PODQ_ACTIVE = '1') then
    							NEXT_PRB_POD <= PODQ;
    						else
    							NEXT_PRB_POD <= NONE;
    						end if;
    					when PODP =>
    						if (PODQ_ACTIVE = '1') then
    							NEXT_PRB_POD <= PODQ;
    						else
    							NEXT_PRB_POD <= NONE;
    						end if;
    					when PODQ =>
    						NEXT_PRB_POD <= NONE;
    					when NONE =>
    						null;
    					end case;
    				end if;
    -- END GET_NEXT_PRB_POD();
    			when DATA_HOLD =>
    				if ((DMA_CNT_ONE = '1') and (CURR_PRB_POD = NONE)) then    -- if (DMA_COUNTER = 1) ....
    					STATE <= LAST_DMAREQ_LOW;
    					DMACK_N <= '1';
    					DMA_TX_DONE <= '1';
    				elsif (CURR_PRB_POD = NONE) then
    					STATE <= NEXT_ADR;
						DMA_COUNTER <= DMA_COUNTER - 1;
    				else
    					STATE <= CHK_DMAREQ1;
    				end if;
        		when DMAREQ_LOW =>
					DMACK_N <= '1';
        			if (DMAREQ_SYNCED = '1') then
        				STATE <= SETUP_DATA;
						DMACK_N <= '0';
        			end if;
        		when LAST_DMAREQ_LOW =>
        			if (DMAREQ_SYNCED = '0') then
        				STATE <= IDLE;
        			end if;
        		end case;
            end if;
        end if;
	end process;

	HOLD_CNT : process(CLK, RESET)
	begin
		if (RESET = '0') then
			HOLD_COUNTER <= 0;
		elsif (rising_edge(CLK)) then
			if (STATE = IDLE) then
				HOLD_COUNTER <= 0;
			elsif (STATE = WRITE) then
				HOLD_COUNTER <= 13;
			elsif (HOLD_COUNTER > 0) then
				HOLD_COUNTER <= HOLD_COUNTER - 1;
			end if;
		end if;
	end process;

	WR_N <= CPU_WR_N and WR_N_I;

	DATA <= DMA_DATA when WR_N_I = '0' else (others => 'Z');

	D12_CS_N <= D12_CS_N_I;
	RD_N <= CPU_RD_N;

end architecture BEHAV;
