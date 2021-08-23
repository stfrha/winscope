-------------------------------------------------------------------------------
-- File name    :  $Id: MAIN_SIM.vhd,v 1.4 2003/06/12 09:09:07 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: MAIN_SIM.vhd,v $
-- Revision 1.4  2003/06/12 09:09:07  frha
-- Added revision string
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity MAIN_SIM is
end entity MAIN_SIM;

architecture TESTBENCH of MAIN_SIM is
	component SIM_PROBES is
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
	end component SIM_PROBES;

	component D12_DMA_SIM is
		port (
			RESET	: in std_logic;
			TRIGGER	: in std_logic;
			DATA	: in std_logic_vector(7 downto 0);
			WR_N	: in std_logic;
			DMACK_N	: in std_logic;
			EOT_N	: in std_logic;
			DMAREQ	: out std_logic
		);
	end component D12_DMA_SIM;

	component sram IS

	  GENERIC (

		clear_on_power_up: boolean := FALSE;    -- if TRUE, RAM is initialized with zeroes at start of simulation
												-- Clearing of RAM is carried out before download takes place

		download_on_power_up: boolean := TRUE;  -- if TRUE, RAM is downloaded at start of simulation 
      
		trace_ram_load: boolean := TRUE;        -- Echoes the data downloaded to the RAM on the screen
												-- (included for debugging purposes)


		enable_nWE_only_control: boolean := TRUE;  -- Read-/write access controlled by nWE only
												   -- nOE may be kept active all the time



		-- Configuring RAM size

		size:      INTEGER :=  256;  -- number of memory words			FRHA	256
		adr_width: INTEGER :=  8;  -- number of address bits			FRHA	8
		width:     INTEGER :=  8;  -- number of bits per memory word	FRHA	8


		-- READ-cycle timing parameters

		tAA_max:    TIME := 20 NS; -- Address Access Time				FRHA	20 ns
		tOHA_min:   TIME :=  3 NS; -- Output Hold Time					FRHA	3 ns
		tACE_max:   TIME := 20 NS; -- nCE/CE2 Access Time				FRHA	20 ns
		tDOE_max:   TIME := 10 NS; -- nOE Access Time					FRHA	10 ns
		tLZOE_min:  TIME :=  2 NS; -- nOE to Low-Z Output				FRHA	2 ns
		tHZOE_max:  TIME :=  8 NS; --  OE to High-Z Output				FRHA	8 ns
		tLZCE_min:  TIME :=  3 NS; -- nCE/CE2 to Low-Z Output			FRHA	3 ns
		tHZCE_max:  TIME := 10 NS; --  CE/nCE2 to High Z Output			FRHA	10 ns
 

		-- WRITE-cycle timing parameters

		tWC_min:    TIME := 20 NS; -- Write Cycle Time					FRHA	20 ns
		tSCE_min:   TIME := 15 NS; -- nCE/CE2 to Write End				FRHA	15 ns
		tAW_min:    TIME := 15 NS; -- tAW Address Set-up Time to Write End	FRHA	15 ns
		tHA_min:    TIME :=  0 NS; -- tHA Address Hold from Write End	FRHA	0 ns
		tSA_min:    TIME :=  0 NS; -- Address Set-up Time				FRHA	0 ns
		tPWE_min:   TIME := 15 NS; -- nWE Pulse Width					FRHA	15 ns
		tSD_min:    TIME :=  8 NS; -- Data Set-up to Write End			FRHA	8 ns
		tHD_min:    TIME :=  0 NS; -- Data Hold from Write End			FRHA	0 ns
		tHZWE_max:  TIME :=  7 NS; -- nWE Low to High-Z Output			FRHA	7 ns
		tLZWE_min:  TIME :=  2 NS  -- nWE High to Low-Z Output			FRHA	2 ns
	  );

	  PORT (
      
		nCE: IN std_logic := '1';  -- low-active Chip-Enable of the SRAM device; defaults to '1' (inactive)
		nOE: IN std_logic := '1';  -- low-active Output-Enable of the SRAM device; defaults to '1' (inactive)
		nWE: IN std_logic := '1';  -- low-active Write-Enable of the SRAM device; defaults to '1' (inactive)

		A:   IN std_logic_vector(adr_width-1 downto 0); -- address bus of the SRAM device
		D:   INOUT std_logic_vector(width-1 downto 0);  -- bidirectional data bus to/from the SRAM device

		CE2: IN std_logic := '1';  -- high-active Chip-Enable of the SRAM device; defaults to '1'  (active) 


		download: IN boolean := FALSE;    -- A FALSE-to-TRUE transition on this signal downloads the data
										  -- in file specified by download_filename to the RAM

		download_filename: IN string := "sram_load.dat";  -- name of the download source file
														  --            Passing the filename via a port of type
														  -- ********** string may cause a problem with some
														  -- WATCH OUT! simulators. The string signal assigned
														  -- ********** to the port at least should have the
														  --            same length as the default value.
 
		dump: IN boolean := FALSE;       -- A FALSE-to-TRUE transition on this signal dumps
										 -- the current content of the memory to the file
										 -- specified by dump_filename.
		dump_start: IN natural := 0;     -- Written to the dump-file are the memory words from memory address 
		dump_end: IN natural := size-1;  -- dump_start to address dump_end (default: all addresses)

		dump_filename: IN string := "sram_dump.dat"  -- name of the dump destination file
													 -- (See note at port  download_filename)

	  );
	END component sram;

	component C51CPUSIM is
		generic (
			PW          : time := 62500 ps
		);
		port (
	        RESET       : in std_logic;
		    ALE         : out std_logic;
			WR		    : out std_logic;
			RD	        : out std_logic;
			PORT0       : inout std_logic_vector(7 downto 0);
			PORT2		: out std_logic_vector(7 downto 0);
			PORT1		: out std_logic_vector(7 downto 0);
			IRQ_N       : in std_logic
		);
	end component C51CPUSIM;		     	

	component MAIN is
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
	
			LEDS		: out std_logic_vector(7 downto 0)
		);
	end component MAIN;


	signal CLK		: std_logic := '0';
	signal RESET	: std_logic := '0';
	signal GEN_SMP_CLK	: std_logic;
	signal SMP_CLK	: std_logic;
	signal DATA		: std_logic_vector(7 downto 0);
	signal WR_n		: std_logic;
	signal RD_n		: std_logic;
	signal ALE		: std_logic;
	signal PRBA_IN	: std_logic_vector(7 downto 0);
	signal PRBB_IN	: std_logic_vector(7 downto 0);
	signal PODP_IN	: std_logic_vector(7 downto 0);
	signal PODQ_IN	: std_logic_vector(7 downto 0);
	signal DMA_TRIGGER	: std_logic;
	signal PORT1	: std_logic_vector(7 downto 0);
	signal D12_WR_N	: std_logic;
	signal DMACK_N	: std_logic;
	signal DMAREQ	: std_logic;
	signal EOT_N	: std_logic;
	signal ADDR_COUNT	: unsigned(16 downto 0);
	signal MEM_E0_N	: std_logic;
	signal MEM_RD_N	: std_logic;
	signal DUMP_SRAM    : boolean;

	signal PRBA_MEM	: std_logic_vector(7 downto 0);
	signal PRBB_MEM	: std_logic_vector(7 downto 0);
	signal PODP_MEM	: std_logic_vector(7 downto 0);
	signal PODQ_MEM	: std_logic_vector(7 downto 0);

	signal PRBA_OE_N    : std_logic;
	signal PRBB_OE_N    : std_logic;
	signal PODP_OE_N    : std_logic;
	signal PODQ_OE_N    : std_logic;

	signal IRQ_N        : std_logic;

begin
	EOT_N <= 'H';		-- Assume pullup on EOT_N

	DMA_TRIGGER <= PORT1(0);

	DUMP_SRAM <= true when PORT1(2) = '1' else false;

--	PRBA_MEM <= PRBA_IN after 1 ps;
--  PRBB_MEM <= PRBB_IN after 1 ps;
--	PODP_MEM <= PODP_IN after 1 ps;
--	PODQ_MEM <= PODQ_IN after 1 ps;


	SPRB : SIM_PROBES port map (
		CLK => SMP_CLK,
		RESET => RESET,
		CHA => PRBA_IN,
		CHA_EN_N => PRBA_OE_N,
		CHB => PRBB_IN,
		CHB_EN_N => PRBB_OE_N,
		PDP => PODP_IN,
		PDP_EN_N => PODP_OE_N,
		PDQ => PODQ_IN,
		PDQ_EN_N => PODQ_OE_N
	);

	DMA : D12_DMA_SIM port map (
		RESET => RESET,
		TRIGGER	=> DMA_TRIGGER,
		DATA => DATA,
		WR_N => D12_WR_N,
		DMACK_N => DMACK_N,
		EOT_N => EOT_N,
		DMAREQ => DMAREQ
	);

	PRBA_RAM : sram 
	generic map (
	    clear_on_power_up => true,
		download_on_power_up => false,
		trace_ram_load => false,
		enable_nWE_only_control => true,
		size => 256,
		adr_width => 8,
		width => 8,
		tAA_max => 20 ns,
		tOHA_min => 3 ns,
		tACE_max => 20 ns,
		tDOE_max => 10 ns,
		tLZOE_min => 2 ns,
		tHZOE_max => 8 ns,
		tLZCE_min => 3 ns,
		tHZCE_max => 10 ns,
		tWC_min => 20 ns,
		tSCE_min => 15 ns,
		tAW_min => 15 ns,
		tHA_min => 0 ns,
		tSA_min => 0 ns,
		tPWE_min => 15 ns,
		tSD_min => 8 ns,
		tHD_min => 0 ns,
		tHZWE_max => 7 ns,
		tLZWE_min => 2 ns
	  )
	port map (
		nCE => MEM_E0_N,
		nOE => MEM_RD_N,
		nWE => SMP_CLK,
		A => std_logic_vector(ADDR_COUNT(7 downto 0)),
		D => PRBA_IN,
		dump => DUMP_SRAM,
	    dump_filename => "prba_dump.txt"
	  );

	PRBB_RAM : sram 
	generic map (
	    clear_on_power_up => true,
		download_on_power_up => false,
		trace_ram_load => false,
		enable_nWE_only_control => true,
		size => 256,
		adr_width => 8,
		width => 8,
		tAA_max => 20 ns,
		tOHA_min => 3 ns,
		tACE_max => 20 ns,
		tDOE_max => 10 ns,
		tLZOE_min => 2 ns,
		tHZOE_max => 8 ns,
		tLZCE_min => 3 ns,
		tHZCE_max => 10 ns,
		tWC_min => 20 ns,
		tSCE_min => 15 ns,
		tAW_min => 15 ns,
		tHA_min => 0 ns,
		tSA_min => 0 ns,
		tPWE_min => 15 ns,
		tSD_min => 8 ns,
		tHD_min => 0 ns,
		tHZWE_max => 7 ns,
		tLZWE_min => 2 ns
	  )
	port map (
		nCE => MEM_E0_N,
		nOE => MEM_RD_N,
		nWE => SMP_CLK,
		A => std_logic_vector(ADDR_COUNT(7 downto 0)),
		D => PRBB_IN,
		dump => DUMP_SRAM,
	    dump_filename => "prbb_dump.txt"
	  );

	PODP_RAM : sram 
	generic map (
	    clear_on_power_up => true,
		download_on_power_up => false,
		trace_ram_load => false,
		enable_nWE_only_control => true,
		size => 256,
		adr_width => 8,
		width => 8,
		tAA_max => 20 ns,
		tOHA_min => 3 ns,
		tACE_max => 20 ns,
		tDOE_max => 10 ns,
		tLZOE_min => 2 ns,
		tHZOE_max => 8 ns,
		tLZCE_min => 3 ns,
		tHZCE_max => 10 ns,
		tWC_min => 20 ns,
		tSCE_min => 15 ns,
		tAW_min => 15 ns,
		tHA_min => 0 ns,
		tSA_min => 0 ns,
		tPWE_min => 15 ns,
		tSD_min => 8 ns,
		tHD_min => 0 ns,
		tHZWE_max => 7 ns,
		tLZWE_min => 2 ns
	  )
	port map (
		nCE => MEM_E0_N,
		nOE => MEM_RD_N,
		nWE => SMP_CLK,
		A => std_logic_vector(ADDR_COUNT(7 downto 0)),
		D => PODP_IN,
		dump => DUMP_SRAM,
	    dump_filename => "podp_dump.txt"
	  );

	PODQ_RAM : sram 
	generic map (
	    clear_on_power_up => true,
		download_on_power_up => false,
		trace_ram_load => false,
		enable_nWE_only_control => true,
		size => 256,
		adr_width => 8,
		width => 8,
		tAA_max => 20 ns,
		tOHA_min => 3 ns,
		tACE_max => 20 ns,
		tDOE_max => 10 ns,
		tLZOE_min => 2 ns,
		tHZOE_max => 8 ns,
		tLZCE_min => 3 ns,
		tHZCE_max => 10 ns,
		tWC_min => 20 ns,
		tSCE_min => 15 ns,
		tAW_min => 15 ns,
		tHA_min => 0 ns,
		tSA_min => 0 ns,
		tPWE_min => 15 ns,
		tSD_min => 8 ns,
		tHD_min => 0 ns,
		tHZWE_max => 7 ns,
		tLZWE_min => 2 ns
	  )
	port map (
		nCE => MEM_E0_N,
		nOE => MEM_RD_N,
		nWE => SMP_CLK,
		A => std_logic_vector(ADDR_COUNT(7 downto 0)),
		D => PODQ_IN,
		dump => DUMP_SRAM,
	    dump_filename => "podq_dump.txt"
	  );

	CPUSIM1 : C51CPUSIM port map (
	    RESET => RESET,
	    ALE => ALE,
		WR => WR_n,
		RD => RD_n,
		PORT0 => DATA,
		PORT1 => PORT1,
		IRQ_N => IRQ_N
	);

	SMP_CLK <= GEN_SMP_CLK after 7 ns;

	MAIN1 : MAIN port map (
		CLK => CLK,
		RESET => RESET,
		GEN_SMP_CLK => GEN_SMP_CLK,
		SMP_CLK => SMP_CLK,
		DATA => DATA,
		CPU_WR_n => WR_n,
		CPU_RD_n => RD_n,
        CPU_INT_N => IRQ_N,
		WR_n => D12_WR_N,
		ALE => ALE,
		PRBA_IN => PRBA_IN,
		PRBA_OE_N => PRBA_OE_N,
		PRBB_IN => PRBB_IN,
		PRBB_OE_N => PRBB_OE_N,
		PODP_IN => PODP_IN,
		PODP_OE_N => PODP_OE_N,
		PODQ_IN => PODQ_IN,
		PODQ_OE_N => PODQ_OE_N,
		EXT_CLK => '0',
		DMAREQ => DMAREQ,
		DMACK => DMACK_N,
		EOT_N => EOT_N,
		ADDR_COUNT => ADDR_COUNT,
		MEM_G_N => MEM_RD_N,
		MEM_E0_N => MEM_E0_N
	);

	RESET <= '0', '1' after 200 ns;

	CLK <= not(CLK) after 16.66666666666666 ns;

end architecture TESTBENCH;

