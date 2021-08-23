-------------------------------------------------------------------------------
-- File name    :  $Id: MAIN.vhd,v 1.8 2003/06/16 13:27:51 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: MAIN.vhd,v $
-- Revision 1.8  2003/06/16 13:27:51  frha
-- Fixed bug with SM_FREE, new version 1.6
--
-- Revision 1.7  2003/06/12 12:30:35  frha
-- Added CAL_PULSE of 1kHz and TRIGG_LED
--
-- Revision 1.6  2003/06/12 09:33:21  frha
-- Added CAL_PULSE of 1kHz
--
-- Revision 1.5  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity SWEEP_RESTART is
	port (
		CLK             : in std_logic;
        RESET           : in std_logic; 
        SAVE_LOAD_N     : in std_logic;
        DMA_TX_DONE     : in std_logic;
        NEXT_STATE      : in std_logic;
        SAVE_LOAD_N_Q   : out std_logic
    );
end SWEEP_RESTART;

architecture BEHAV of SWEEP_RESTART is
    signal  OVERRIDE_S_L    : std_logic;
    signal  DMA_TX_DONE_OLD : std_logic;
    signal  NEXT_STATE_OLD  : std_logic;
    signal  SAVE_LOAD_SYNC  : std_logic;
begin
    process(CLK, RESET)
    begin
        if (RESET = '0') then
            OVERRIDE_S_L <= '0';
            DMA_TX_DONE_OLD <= '0';
            NEXT_STATE_OLD <= '0';
            SAVE_LOAD_SYNC <= '0';
        elsif (rising_edge(CLK)) then
            DMA_TX_DONE_OLD <= DMA_TX_DONE;
            NEXT_STATE_OLD <= NEXT_STATE;
            SAVE_LOAD_SYNC <= SAVE_LOAD_N;
            if ((DMA_TX_DONE = '1' and DMA_TX_DONE_OLD = '0') or (NEXT_STATE = '1' and NEXT_STATE_OLD = '0')) then
                OVERRIDE_S_L <= '1';
            elsif (SAVE_LOAD_SYNC = '1') then
                OVERRIDE_S_L <= '0';
            end if;
        end if;
    end process;

    SAVE_LOAD_N_Q <= SAVE_LOAD_N when OVERRIDE_S_L = '0' else '1';

end architecture BEHAV;

            
            
            












library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library UNISIM;
use UNISIM.VCOMPONENTS.all;

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
		ADDR_COUNT	: out unsigned(16 downto 0);

		PRBA_IN		: in std_logic_vector(7 downto 0);
		PRBA_OE_N   : out std_logic;
		PRBB_IN		: in std_logic_vector(7 downto 0);
		PRBB_OE_N   : out std_logic;
		PODP_IN		: in std_logic_vector(7 downto 0);
		PODP_OE_N   : out std_logic;
		PODQ_IN		: in std_logic_vector(7 downto 0);
		PODQ_OE_N   : out std_logic;

		PRBA_GAIN	: out std_logic_vector(3 downto 0);
		PRBB_GAIN	: out std_logic_vector(3 downto 0);
		PRBA_AC		: out std_logic;
		PRBB_AC		: out std_logic;
		PRBA_GND	: out std_logic;
		PRBB_GND	: out std_logic;

		LEDS		: out std_logic_vector(7 downto 0);
		CAL_PULSE   : out std_logic
	);
end MAIN;

architecture STRUCT of MAIN is
	component CLK_GEN is
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
	end component CLK_GEN;
	

	component CPU_REGISTERS is
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

	        IRQ_N           : out std_logic;

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
	end component CPU_REGISTERS;

	component TRIGGER is
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
	end component TRIGGER;

	component ADDR_COUNT_C is
		port (
			SMP_CLK			: in std_logic;
			RESET			: in std_logic;
			BUFFER_SIZE		: in unsigned(16 downto 0);
			ADDR_COUNT		: out unsigned(16 downto 0)
		);
	end component ADDR_COUNT_C;
	
	component DMA_CONTROLER is
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
	end component DMA_CONTROLER;

    component SWEEP_RESTART is
    	port (
    		CLK             : in std_logic;
            RESET           : in std_logic; 
            SAVE_LOAD_N     : in std_logic;
            DMA_TX_DONE     : in std_logic;
            NEXT_STATE      : in std_logic;
            SAVE_LOAD_N_Q   : out std_logic
        );
    end component SWEEP_RESTART;

	component CONSTANTS is
		port (
			MEM_BS0_N	:	out std_logic;
			MEM_BS1_N	:	out std_logic;
			MEM_BS2_N	:	out std_logic;
			MEM_BS3_N	:	out std_logic
		);
	end component CONSTANTS;

    component TDO
        port (
            O : in std_logic := 'X'
        );
    end component TDO;

    component OBUFT
        port(
            I : in std_logic := 'X';
            O : out std_logic;
            T : in std_logic := 'X'
        );
    end component;

	signal	CLK_CHANGE	: std_logic;
	signal	CLK_TOGGLE	: std_logic_vector(23 downto 0);
	signal	CLK_RESET	: std_logic_vector(23 downto 0);
	signal	CLK_SELECT	: std_logic;

	signal	LOAD_INC_PULSE	: std_logic;

	signal	PRBA_TRG_LEVEL	: std_logic_vector(7 downto 0);
	signal	PRBB_TRG_LEVEL	: std_logic_vector(7 downto 0);
	signal	PRB_TRG_SOURCE	: std_logic;
	signal	PRB_TRG_SLOPE	: std_logic;

	signal	PODP_TRG_PTRN	: std_logic_vector(7 downto 0);
	signal	PODQ_TRG_PTRN	: std_logic_vector(7 downto 0);
	signal	PODP_PTRN_VALID	: std_logic_vector(7 downto 0);
	signal	PODQ_PTRN_VALID	: std_logic_vector(7 downto 0);
	signal	PODP_EDGE_SLOPE	: std_logic_vector(7 downto 0);
	signal	PODQ_EDGE_SLOPE	: std_logic_vector(7 downto 0);
	signal	PODP_EDGE_VALID	: std_logic_vector(7 downto 0);
	signal	PODQ_EDGE_VALID	: std_logic_vector(7 downto 0);

	signal	TRIGG_SETUP		: std_logic_vector(7 downto 0);

	signal	TRIGG_DELY		: unsigned(16 downto 0);
	signal	BUFFER_SIZE		: unsigned(16 downto 0);

	signal	NEXT_STATE		: std_logic;
    signal	NEXT_STATE_L	: std_logic;
	signal	SWEEP_MODE		: std_logic_vector(1 downto 0);
	signal	SAVE_LOAD_N		: std_logic;
	signal	SWEEP_STATE		: std_logic_vector(2 downto 0);

	signal	D12_CS_N_I		: std_logic;

	signal	PRBA_ACTIVE		: std_logic;
	signal	PRBB_ACTIVE		: std_logic;
	signal	PODP_ACTIVE		: std_logic;
	signal	PODQ_ACTIVE		: std_logic;

	signal	DMA_TX_DONE		: std_logic;
	signal	SWEEP_DONE		: std_logic;
	signal	DID_TRIGG		: std_logic;

	signal	SAVE_LOAD_N_Q	: std_logic;

	signal	PRBA_D  		: std_logic_vector(7 downto 0);
	signal	PRBB_D	    	: std_logic_vector(7 downto 0);
	signal	PODP_D		    : std_logic_vector(7 downto 0);
	signal	PODQ_D  		: std_logic_vector(7 downto 0);

	signal	LEDS_ACT        : std_logic;
	signal	LEDS_RESET 		: std_logic_vector(7 downto 0);
	signal	LEDS_COUNT 		: unsigned(7 downto 0);

	signal  TRIGG_LED       : std_logic;
	signal  TRIGG_LED_B     : std_logic;
    signal  VCC             : std_logic := '1';
    signal  GND             : std_logic := '0';
begin
    
    VCC <= '1';
    GND <= '0';

    process(SMP_CLK, RESET)
    begin
        if (RESET = '0') then
            LEDS_COUNT <= (others => '0');
        elsif (rising_edge(SMP_CLK)) then
            if (LEDS_ACT = '1') then
                LEDS_COUNT <= unsigned(LEDS_RESET);
            else
                LEDS_COUNT <= LEDS_COUNT + 1;
            end if;
        end if;
    end process;

    LEDS <= std_logic_vector(LEDS_COUNT);
    
    process(SMP_CLK, RESET)
    begin
        if (RESET = '0') then
            PRBA_D <= (others => '0');
            PRBB_D <= (others => '0');
            PODP_D <= (others => '0');
            PODQ_D <= (others => '0');
        elsif (rising_edge(SMP_CLK)) then
            PRBA_D <= PRBA_IN;
            PRBB_D <= PRBB_IN;
            PODP_D <= PODP_IN;
            PODQ_D <= PODQ_IN;
        end if;
    end process;
	
	MEM_G_N <= SAVE_LOAD_N;

    PRBA_OE_N <= not SAVE_LOAD_N;
    PRBB_OE_N <= not SAVE_LOAD_N;
    PODP_OE_N <= not SAVE_LOAD_N;
    PODQ_OE_N <= not SAVE_LOAD_N;

    SWEEP_DONE <= not SAVE_LOAD_N;

	CKG : CLK_GEN port map (
		CLK => CLK,
		RESET => RESET,
		CLK_CHANGE => CLK_CHANGE,
		CLK_TOGGLE => CLK_TOGGLE,
		CLK_RESET => CLK_RESET,

		EXT_CLK => EXT_CLK,
		CLK_SELECT => CLK_SELECT,
		SAVE_LOAD_N => SAVE_LOAD_N_Q,
		GEN_SMP_CLK => GEN_SMP_CLK,

		LOAD_INC_PULSE => LOAD_INC_PULSE,
		MEM_E0_N => MEM_E0_N,
		MEM_E1_N => MEM_E1_N,
		CAL_PULSE => CAL_PULSE
	);

	CPR : CPU_REGISTERS port map (
		CLK => CLK,
		SMP_CLK => SMP_CLK,
		RESET => RESET,

		ALE => ALE,
		DATA => DATA,
		CPU_RD_N => CPU_RD_N,
		CPU_WR_N => CPU_WR_N,

		D12_CS_N_I => D12_CS_N_I,

		SWEEP_STATE => SWEEP_STATE,
		DMA_TX_DONE => DMA_TX_DONE,
		SWEEP_DONE => SWEEP_DONE,
		DID_TRIGG => DID_TRIGG,
		PRBA_TRG_LEVEL => PRBA_TRG_LEVEL,
		PRBB_TRG_LEVEL => PRBB_TRG_LEVEL,
		PRB_TRG_SOURCE => PRB_TRG_SOURCE,
		PRB_TRG_SLOPE => PRB_TRG_SLOPE,
		PODP_TRG_PTRN => PODP_TRG_PTRN,
		PODQ_TRG_PTRN => PODQ_TRG_PTRN,
		PODP_PTRN_VALID	=> PODP_PTRN_VALID,
		PODQ_PTRN_VALID	=> PODQ_PTRN_VALID,
		PODP_EDGE_SLOPE	=> PODP_EDGE_SLOPE,
		PODQ_EDGE_SLOPE	=> PODQ_EDGE_SLOPE,
		PODP_EDGE_VALID	=> PODP_EDGE_VALID,
		PODQ_EDGE_VALID	=> PODQ_EDGE_VALID,
		TRIGG_SETUP	=> TRIGG_SETUP,
		TRIGG_DELY => TRIGG_DELY,
		BUFFER_SIZE => BUFFER_SIZE,

		PRBA_ACTIVE => PRBA_ACTIVE,
		PRBB_ACTIVE => PRBB_ACTIVE,
		PODP_ACTIVE => PODP_ACTIVE,
		PODQ_ACTIVE => PODQ_ACTIVE,

		PRBA_DATA => PRBA_D,
		PRBB_DATA => PRBB_D,
		PODP_DATA => PODP_D,
		PODQ_DATA => PODQ_D,

		CLK_CHANGE => CLK_CHANGE,
		CLK_TOGGLE => CLK_TOGGLE,
		CLK_RESET => CLK_RESET,
		CLK_SELECT => CLK_SELECT,

        IRQ_N => CPU_INT_N,

		PRBA_GAIN => PRBA_GAIN,
		PRBB_GAIN => PRBB_GAIN,
		PRBA_AC => PRBA_AC,
		PRBB_AC => PRBB_AC,
		PRBA_GND => PRBA_GND,
		PRBB_GND => PRBB_GND,

		LEDS => LEDS_RESET,
		LEDS_ACT => LEDS_ACT,
		
		TRIGG_LED => TRIGG_LED,
		
		NEXT_STATE => NEXT_STATE,
		NEXT_STATE_L => NEXT_STATE_L,
		SWEEP_MODE => SWEEP_MODE
	);


	TRG : TRIGGER port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		PRBA_IN => PRBA_D,
		PRBB_IN => PRBB_D,
		PRBA_TRG_LEVEL => PRBA_TRG_LEVEL,
		PRBB_TRG_LEVEL => PRBB_TRG_LEVEL,
		PRB_TRG_SOURCE => PRB_TRG_SOURCE,
		PRB_TRG_SLOPE => PRB_TRG_SLOPE,
	
		PODP_IN => PODP_D,
		PODQ_IN => PODQ_D,
		PODP_TRG_PTRN => PODP_TRG_PTRN,
		PODQ_TRG_PTRN => PODQ_TRG_PTRN,
		PODP_PTRN_VALID	=> PODP_PTRN_VALID,
		PODQ_PTRN_VALID	=> PODQ_PTRN_VALID,
		PODP_EDGE_SLOPE	=> PODP_EDGE_SLOPE,
		PODQ_EDGE_SLOPE	=> PODQ_EDGE_SLOPE,
		PODP_EDGE_VALID	=> PODP_EDGE_VALID,
		PODQ_EDGE_VALID	=> PODQ_EDGE_VALID,
	
		EXT_TRIGG => EXT_CLK,
		TRIGG_SETUP	=> TRIGG_SETUP,
	
		TRIGG_DELY => TRIGG_DELY,
		BUFFER_SIZE => BUFFER_SIZE,
	
		NEXT_STATE => NEXT_STATE,
		SWEEP_MODE => SWEEP_MODE,
		SAVE_LOAD_N => SAVE_LOAD_N,
		DID_TRIGG => DID_TRIGG,
		SWEEP_STATE => SWEEP_STATE
	);

	ARC : ADDR_COUNT_C port map (
		SMP_CLK => SMP_CLK,
		RESET => RESET,
		BUFFER_SIZE => BUFFER_SIZE,
		ADDR_COUNT => ADDR_COUNT
	);

	DMA : DMA_CONTROLER port map (
		CLK => CLK,
		RESET => RESET,

		CPU_RD_N => CPU_RD_N,
		CPU_WR_N => CPU_WR_N,
		D12_CS_N_I => D12_CS_N_I,

		BUFFER_SIZE => BUFFER_SIZE,
		PRBA_ACTIVE => PRBA_ACTIVE,
		PRBB_ACTIVE => PRBB_ACTIVE,
		PODP_ACTIVE => PODP_ACTIVE,
		PODQ_ACTIVE => PODQ_ACTIVE,
		PRBA_IN => PRBA_IN,             -- SMP_CLK is not running during DMA transfer => no clock to buffer flipflop
		PRBB_IN => PRBB_IN,
		PODP_IN => PODP_IN,
		PODQ_IN => PODQ_IN,

        SAVE_LOAD_N => SAVE_LOAD_N,

		DMAREQ => DMAREQ,

		RD_N => RD_N,
		WR_N => WR_N,
		D12_CS_N => D12_CS_N,

		DMACK_N => DMACK,
        EOT_N => EOT_N,

		DMA_TX_DONE => DMA_TX_DONE,
		DATA => DATA,
		LOAD_INC_PULSE => LOAD_INC_PULSE
	);

    SWR : SWEEP_RESTART port map (
		CLK => CLK,
		RESET => RESET,

		SAVE_LOAD_N => SAVE_LOAD_N,
		DMA_TX_DONE => DMA_TX_DONE,
        NEXT_STATE => NEXT_STATE_L,
        SAVE_LOAD_N_Q => SAVE_LOAD_N_Q
    );

	CNS : CONSTANTS port map (
		MEM_BS0_N => MEM_BS0_N,
		MEM_BS1_N => MEM_BS1_N,
		MEM_BS2_N => MEM_BS2_N,
		MEM_BS3_N => MEM_BS3_N
	);

    TDO1 : TDO port map (O => TRIGG_LED_B);

    OBUFT1 :  OBUFT port map (
        I => TRIGG_LED, 
        O => TRIGG_LED_B,
        T => GND
    );
end architecture STRUCT;
