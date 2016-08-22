namespace StructLib
{
    using ComponentAce.Compression.Libs.zlib;
    using System;
    using System.IO;

    public class DeCompress
    {
        public static byte[] Compress(byte[] inputBytes)
        {
            byte[] buffer;
            using (MemoryStream stream = new MemoryStream())
            {
                using (ZOutputStream stream2 = new ZOutputStream((Stream) stream, -1))
                {
                    stream2.Write(inputBytes, 0, inputBytes.Length);
                    stream2.finish();
                    buffer = stream.ToArray();
                }
            }
            return buffer;
        }

        public static byte[] Decompress(byte[] inputBytes)
        {
            byte[] buffer;
            using (MemoryStream stream = new MemoryStream())
            {
                using (ZOutputStream stream2 = new ZOutputStream((Stream) stream))
                {
                    stream2.Write(inputBytes, 0, inputBytes.Length);
                    stream2.finish();
                    buffer = stream.ToArray();
                }
            }
            return buffer;
        }
    }
}

