#include <stdint.h>
#include "stm32f3xx.h"
#include "stm32f3xx_conf.h"
#include "system.h"
#include "ui.h"
#include "drivers/debug_pin.h"

using namespace wavesynth;

Ui ui;
System sys;
TempoKeeper tempoKeeper;
Generator generator;
Envelope envelope;
Modulator xModulator;

void Init() {
	sys.Init();
	DebugPin::Init();
	ui.Init(&tempoKeeper, &generator, &envelope, &xModulator);
	sys.StartTimers();
}

int main(void)
{
	Init();
	while (1) {
		ui.DoEvents();
	}
	return 0;
}

//Interrupt handlers ====================================================================
extern "C" {
	//1kHz Handler
	void SysTick_Handler(void) {
		TIC9
		ui.Poll();
		TOC9
	}
	
	//48kHz (SampleRate) Handler
	void TIM6_DAC_IRQHandler(void) {
		TIC8
		if(LL_TIM_IsActiveFlag_UPDATE(TIM6) == 1) {
			LL_TIM_ClearFlag_UPDATE(TIM6);
		}
		ui.ProcessNextSample();
		TOC8
	}
}