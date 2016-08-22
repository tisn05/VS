using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;
using System.IO;

namespace tisn.cdr
{
    public class RiffChunk : RIFFBase
    {
        protected String tmpFileName = null;
        protected String tmpPath = ".\\tmp\\";
        public RiffChunk(CDRFile p)
            : base(p)
        {
            this.Type = RiffType.Chunk;
            if (!Directory.Exists(tmpPath))
            {
                Directory.CreateDirectory(tmpPath);
            }
        }
        public byte[] GetContent()
        {
            MemoryStream ms = new MemoryStream();
            FileStream fs = File.OpenRead(tmpPath + tmpFileName);
            int size = 2048;
            byte[] tmp = new byte[2048];
            while (true)
            {
                size = fs.Read(tmp, 0, 2048);
                if (size > 0)
                {
                    ms.Write(tmp, 0, size);
                }
                else
                {
                    break;
                }
            }
            byte[] o = ms.ToArray();
            fs.Close();
            ms.Close();
            return o;
        }
        public override int Write(System.IO.BinaryWriter bw)
        {
            long p = bw.BaseStream.Position;
            bw.BaseStream.Position += this.content.Length;
            return (int)(bw.BaseStream.Position - p);
        }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            this.postoin = br.BaseStream.Position - 8;
            this.length = len;
            br.BaseStream.Position += len;
        }/// <summary>
        /// 字节数组转16进制字符串
        /// </summary>
        /// <param name="bytes"></param>
        /// <returns></returns>
        public static string byteToHexStr(byte[] bytes)
        {
            string returnStr = "";
            if (bytes != null)
            {
                for (int i = 0; i < bytes.Length; i++)
                {
                    returnStr += bytes[i].ToString("X2");
                }
            }
            return returnStr;
        }
    }
}
