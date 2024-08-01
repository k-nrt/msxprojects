using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace ihx2bin
{
    internal class Program
    {
        static int Main(string[] args)
        {
            string inputPathName = null;
            string outputPathName = null;
            int offset = 0;
            int size = 65536;

            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-input")
                {
                    if (args.Length <= i+1)
                    {
                        Console.Error.WriteLine("Not found input path.");
                        return 1;
                    }
                    else
                    {
                        inputPathName = args[i+1];
                        i++;
                    }
                }
                else if (args[i] == "-output")
                {
                    if (args.Length <= i+1)
                    {
                        Console.Error.WriteLine("Not found output path.");
                        return 1;
                    }
                    else
                    {
                        outputPathName = args[i+1];
                        i++;
                    }
                }
                if (args[i] == "-offset")
                {
                    if (args.Length <= i+1)
                    {
                        Console.Error.WriteLine("Not found offset value.");
                        return 1;
                    }
                    else if (!int.TryParse(args[i+1], out offset))
                    {
                        Console.Error.WriteLine("Can not parse offset value.");
                        return 1;
                    }
                    i++;
                }
                else if (args[i] == "-size")
                {
                    if (args.Length <= i+1)
                    {
                        Console.Error.WriteLine("Not found size value.");
                        return 1;
                    }
                    else if (!int.TryParse(args[i+1], out size))
                    {
                        Console.Error.WriteLine("Can not parse size value.");
                        return 1;
                    }
                    i++;
                }
            }

            if (inputPathName == null)
            {
                Console.Error.WriteLine("Input path is not specified.");
                return 1;
            }

            if (outputPathName == null)
            {
                Console.Error.WriteLine("Output path is not specified.");
                return 1;
            }

            if (offset < 0 || 65536 <= offset)
            {
                Console.Error.WriteLine("Output offset is out of range.");
                return 1;
            }

            if (size <= 0 || 65536 <= (offset + size))
            {
                Console.Error.WriteLine("Output size is out of range.");
                return 1;
            }

            //, 読み込み.
            StreamReader streamReader = new StreamReader(inputPathName);
            if (streamReader == null)
            {
                Console.Error.WriteLine(string.Format("Can not open {0}", inputPathName));
                return 1;
            }

            IntelHexFileReader intelHexFileReader = new IntelHexFileReader(streamReader);
            streamReader.Close();

            //. 書き出し.
            FileStream fileStream = new FileStream(outputPathName, FileMode.Create, FileAccess.Write);
            if (fileStream == null)
            {
                Console.Error.WriteLine(string.Format("Can not open {0}", outputPathName));
                return 1;
            }
            fileStream.Write(intelHexFileReader.Image, offset, size);
            fileStream.Close();

            Console.WriteLine("Done.");
            return 0;
        }
    }
}
