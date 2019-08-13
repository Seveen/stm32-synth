import numpy as np
sawwave = np.linspace(0, 255, 256)

saw_int16 = map(lambda x: int(np.round(x*16)), sawwave)

with open('saw.cc', 'w') as f:
    f.write('#include "waves.h"\n')
    f.write("uint16_t sawtable[256] = {\n")
    for sample in saw_int16:
        f.write("   %s,\n" % sample)
    f.write("};\n")