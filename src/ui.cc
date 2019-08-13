#include "ui.h"

namespace wavesynth {
    void Ui::Init(TempoKeeper* tempoKeeper, Generator* generator, Envelope* envelope, Modulator* xModulator) {
        this->generator = generator;
        this->tempoKeeper = tempoKeeper;
        this->envelope = envelope;
        this->xModulator = xModulator; 

        buffer = 0;

        srTimer.Init();
        matrix.Init();
        led.Init();
        led.On();
        audioOut.Init(&buffer);
        dac.Init(&buffer);
        adc.Init();

        tempoKeeper->Init();
        generator->Init();
        envelope->Init();
        xModulator->Init();

        queue.Init();
        dac.StartDAC();
        adc.StartADC();

        //Debug
        generator->SetFrequency(50);

        xModulator->SetWaveToSaw();
        //Debug
    }

    void Ui::Poll() {
        system_clock.Tick();
        matrix.Debounce();
        //Queue le clavier
        for (uint8_t col=0;col<MATRIX_COL;++col) {
            for (uint8_t row=0;row<MATRIX_ROW;++row) {
                if (matrix.just_pressed(col, row)) {
                    queue.AddEvent(CONTROL_SWITCH, row*MATRIX_COL+col, 0);
                } else if (matrix.pressed(col, row)) {
                    queue.AddEvent(CONTROL_SWITCH, row*MATRIX_COL+col, 1);
                } else if (matrix.released(col,row)) {
                    queue.AddEvent(CONTROL_SWITCH, row*MATRIX_COL+col, -1);
                }
            }
        }
        //Queue l'update de l'envelope et des modulators;
        queue.AddEvent(ENVELOPE_UPDATE, 0, 0);
        queue.AddEvent(MODULATOR_UPDATE, 0, 0);

        //C'est maintenant que tu donne du feedback visuel
        if (tempoKeeper->isNowBeat()) {
            led.On();
        } else {
            led.Off();
        }
    }

    void Ui::ProcessNextSample() {
        buffer = generator->ProcessNextBlended();
        audioOut.TransmitSample();
    }

    void Ui::FlushEvents() {
        queue.Flush();
    }

    void Ui::DoEvents() {
        while (queue.available()) {
            TIC7
            Event e = queue.PullEvent();
            if (e.control_type == CONTROL_SWITCH) {
                if (e.data == 0) {
                    OnSwitchPressed(e);
                } else if (e.data == -1) {
                    OnSwitchReleased(e);
                }
            }
            if (e.control_type == ENVELOPE_UPDATE) {
                //Debug
                float amp = adc.GetNormalizedChannelValue(0);
                xModulator->SetAmplitude(amp);
                float off = adc.GetNormalizedChannelValue(1);
                xModulator->SetOffset(off);                
                //Debug

                generator->SetLevel(envelope->Next());
            }
            if (e.control_type == MODULATOR_UPDATE) {
                generator->SetBlendLevel(xModulator->Next());
            }
            TOC7
        }
    }

    void Ui::OnSwitchPressed(const Event& e) {
        switch (e.control_id) {
            //All debug tant que pas sur le full clavier
            case BUTTON_TAPTEMPO:
                tempoKeeper->Tap();
                envelope->SetBeatFrequency(tempoKeeper->getTempoFrequency());
                xModulator->SetBeatFrequency(tempoKeeper->getTempoFrequency());
                envelope->HardSync();
                xModulator->HardSync();
                generator->HardSync();
                break;
            case BUTTON_2:
                xModulator->SetNoteDuration(BEAT);
                xModulator->HardSync();
                break;
            case BUTTON_3:
                xModulator->SetNoteDuration(BEAT_TRIPLET);
                xModulator->HardSync();
                break;
            case BUTTON_4:
                envelope->SetInAttack();
                break;
            default:
                break;
            //All debug
        }
    }

    void Ui::OnSwitchReleased(const Event& e) {
        switch (e.control_id) {
            //All debug tant que pas sur le full clavier
            case BUTTON_4:
                envelope->SetInDecay();
            default:
                break;
            //All debug
        }
    }
}