using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace makedsk
{
    internal class Program
    {
        static int Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.Error.WriteLine("Not specified output path.");
                return 1;
            }

            string outputPathName = args[1];
            int sizeInKB = 720;
            for (int i = 1; i < args.Length; i++)
            {
                if (args[i] == "--output")
                {
                    if (args.Length <= (i+1))
                    {
                        Console.Error.WriteLine("Not found output path.");
                        return 1;                    
                    }

                    outputPathName = args[i+1];
                    i++;
                }
                else if (args[i] == "--size")
                {
                    if (args.Length <= (i+1))
                    {
                        Console.Error.WriteLine("Not found size in KB.");
                        return 1;
                    }
                    else if (!int.TryParse(args[i+1], out sizeInKB))
                    {
                        Console.Error.WriteLine("Can not parse size parameter.");
                        return 1;
                    }
                    i++;
                }
            }

            byte[] diskImage = new byte[sizeInKB*1024];

            //. 書き出し.
            FileStream fileStream = new FileStream(outputPathName, FileMode.Create, FileAccess.Write);
            if (fileStream == null)
            {
                Console.Error.WriteLine(string.Format("Can not open {0}", outputPathName));
                return 1;
            }

            fileStream.Write(diskImage, 0, diskImage.Length);
            fileStream.Close();

            return 0;
        }
    }
}
