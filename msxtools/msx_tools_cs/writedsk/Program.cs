using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace writedsk
{
    internal class Program
    {
        static int Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.Error.WriteLine("Not found target image file.");
                return 1;
            }

            string targetPath = args[0];
            string inputPath = args[1];
            int offset = 0;
            int size = 0;
            int sectorSize = 512;
            int sector = 0;

            for (int i = 2; i < (args.Length - 1); i++)
            {
                if (args[i] == "--offset")
                {
                    if (!int.TryParse(args[i+1], out offset))
                    {
                        Console.Error.WriteLine("Can not parse offset.");
                        return 1;
                    }
                    i++;
                }
                else if (args[i] == "--size")
                {
                    if (!int.TryParse(args[i + 1], out size))
                    {
                        Console.Error.WriteLine("Can not parse size.");
                        return 1;
                    }
                    i++;
                }
                else if (args[i] == "--sector-size")
                {
                    if (!int.TryParse(args[i + 1], out sectorSize))
                    {
                        Console.Error.WriteLine("Can not parse sector size.");
                        return 1;
                    }
                    i++;
                }
                else if (args[i] == "--sector")
                {
                    if (!int.TryParse(args[i + 1], out sector))
                    {
                        Console.Error.WriteLine("Can not parse sector.");
                        return 1;
                    }
                    i++;
                }
            }

            FileStream file = new FileStream(targetPath, FileMode.Open, FileAccess.Read);
            if (file == null)
            {
                Console.Error.WriteLine(string.Format("Can not open {0}", targetPath));
                return 1;
            }

            int targetLength = (int)file.Length;
            if (targetLength <= 0)
            {
                Console.Error.WriteLine(string.Format("Invalid target file size {0}", targetPath));
                return 1;
            }

            byte[] targetImage = new byte[targetLength];
            file.Read(targetImage, 0, targetLength);
            file.Close();

            file = new FileStream(inputPath, FileMode.Open, FileAccess.Read);
            if (file == null)
            {
                Console.Error.WriteLine(string.Format("Can not open {0}", inputPath));
                return 1;
            }

            int inputLength = (int)file.Length;
            if (inputLength <= 0)
            {
                Console.Error.WriteLine(string.Format("Invalid input file size {0}", inputPath));
                return 1;
            }

            byte[] inputImage = new byte[inputLength];
            file.Read(inputImage, 0, inputLength);
            file.Close();

            if (size <= 0)
            {
                size = inputLength;
            }

            if (offset < 0)
            {
                Console.Error.WriteLine(string.Format("Invalid offset {0}", offset));
                return 1;
            }

            if (inputLength < (offset + size))
            {
                Console.Error.WriteLine(string.Format("Invalid size {0}", size));
                return 1;
            }

            if (targetLength < (sector * sectorSize + size))
            {
                Console.Error.WriteLine("Invalid range.");
                return 1;
            }

            for (int i = 0; i < size; i++)
            {
                targetImage[sector*sectorSize + i] = inputImage[offset + i];
            }

            file = new FileStream(targetPath, FileMode.Open, FileAccess.Write);
            if (file == null)
            {
                Console.Error.WriteLine(string.Format("Can not update {0}", targetPath));
                return 1;
            }
            file.Write(targetImage, 0, targetLength);
            file.Close();

            return 0;
        }
    }
}
