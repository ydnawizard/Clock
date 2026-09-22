#include <max_driver.h>

////////LINE CONTROL\\\\\\
//CLOCK -> PA5
//MOSI -> PA6
//CS -> PA7
static void SET_CLK_HIGH(void)
{
	GPIOA->BSRR = (1 << 5);
}

static void SET_CLK_LOW(void)
{
	GPIOA->BSRR = (1 << (5 + 16));
}

static void SET_MOSI_HIGH(void)
{
	GPIOA->BSRR = (1 << 6);
}

static void SET_MOSI_LOW(void)
{
	GPIOA->BSRR = (1 << (6+16));
}

void SET_CS_HIGH(void)
{
	GPIOA->BSRR = (1 << 7);
}

void SET_CS_LOW(void)
{
	GPIOA->BSRR = (1 << (7+16));
}

//////DRIVER LOGIC\\\\\\\\
//Generates packet signal and controls CLK
//The MAX7219 reads the rising end of the CLK so
//this sends a 16 bit packet down the MOSI line
//and sets the clock high so the chip knows to read
static void MAX_Send_Data(uint8_t row_addr, uint8_t data)
{
	uint16_t packet = ((uint16_t)row_addr << 8) | data;
	for(int i = 15; i >= 0; i--)
	{
		SET_CLK_LOW();
		if(packet & (1 << i))
		{
			SET_MOSI_HIGH();
		}
		else
		{
			SET_MOSI_LOW();
		}
		for(volatile uint8_t d= 0; d < 5; d++)
		{
			__NOP();
		}
		SET_CLK_HIGH();
		for(volatile uint8_t d = 0; d < 5; d++)
		{
			__NOP();
		}
	}
	SET_CLK_LOW();
}

//Sends data to specific row on specific matrix
void MAX_Chip_Send_Target(
		uint8_t target_chip,
		uint8_t row_addr, 
		uint8_t data
		)
{
	SET_CS_LOW();
	for (int i = 3; i >= 0; i--)
	{
		if(i == target_chip)
		{
			MAX_Send_Data(row_addr, data);
		}
		else
		{
			//If not target chip, write empty values to provide correct
			//input stream offset (16 bits * matrix index)s
			MAX_Send_Data(0x00, 0x00);
		}
	}
	SET_CS_HIGH();
}

//Sends data to specific row on all matrices
void MAX_Chip_Send_All(uint8_t row_addr, uint8_t data)
{
	SET_CS_LOW();
	for (uint8_t i = 0; i < 4; i++)
	{
		MAX_Send_Data(row_addr,data);
	}
	SET_CS_HIGH();
}

void MAX_Init(void)
{
    MAX_Chip_Send_All(0x0F, 0x00); // Disable Display Test
    MAX_Chip_Send_All(0x0C, 0x01); // Normal Operation (Exit Shutdown)
    MAX_Chip_Send_All(0x0B, 0x07); // Scan Limit: 8 Digits / Rows
    MAX_Chip_Send_All(0x09, 0x00); // Decode Mode: No decode (Matrix)
    MAX_Chip_Send_All(0x0A, 0x03); // Intensity / Brightness (0x00 to 0x0F)
    
    MAX_Clear();                   // Clear all LEDs
}

/////// USEFUL FUNCTIONS \\\\\\\\

void MAX_Clear(void)
{
	for (uint8_t i = 1; i <= 8; i++)
	{
		MAX_Chip_Send_All(i, 0x00);
	}
}
