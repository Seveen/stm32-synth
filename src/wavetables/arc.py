import numpy as np

wave = np.linspace(0.0, 1.0, 128)

inv_arcwave = list(map(lambda x: int(np.round(1-np.sqrt(1-x**2)*1024)+1023)*4, wave))
arcwave = list(map(lambda x: int(np.round(np.sqrt(1-x**2)*1024))*4, wave))

total_arcwave = inv_arcwave + arcwave

with open('arc.cc', 'w') as f:
    f.write('#include "waves.h"\n')
    f.write("uint16_t arctable[256] = {\n")
    for sample in total_arcwave:
        f.write("   %s,\n" % sample)
    f.write("};\n")

total_arcwave.reverse()

with open('antiarc.cc', 'w') as f:
    f.write('#include "waves.h"\n')
    f.write("uint16_t antiarctable[256] = {\n")
    for sample in total_arcwave:
        f.write("   %s,\n" % sample)
    f.write("};\n")