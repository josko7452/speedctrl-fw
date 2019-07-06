import sys
import numpy as np

if (len(sys.argv) != 3):
    print("Generate LUT for NCO\n Usage: gen_nco_lut.py lut_size_base output_amplitude\n")
    sys.exit(1)
lut_size_base = int(sys.argv[1])
amplitude = int(sys.argv[2])

if __name__ == "__main__":
    lut_size = 2 ** lut_size_base
    lut = [str(int(amplitude * np.sin(np.pi * 2.0 * i / lut_size)))
           for i in range(lut_size)]
    print("#ifndef NCO_LUT_H\n",
          "#define NCO_LUT_H\n",
          "#define LUT_SIZE ({})\n".format(lut_size),
          "#define LUT_SIZE_BASE ({})\n".format(lut_size_base),
          "#define LUT_INIT {",
          "{}".format(', '.join(lut)),
          "};\n",
          "#endif // NCO_LUT_H")
