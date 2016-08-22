using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace tisn.cdr
{

    public class RiffList : RIFFBase
    {
        public List<RIFFBase> subList = new List<RIFFBase>();
        public RiffList(CDRFile p)
            : base(p)
        {
            this.Type = RiffType.LIST;
        }
        public override int Write(System.IO.BinaryWriter bw)
        {
            long p = bw.BaseStream.Position;
            int len = 0;
            bw.Write(RiffParser.ToFourCC("LIST"));
            bw.Write(0);
            bw.Write(RiffParser.ToFourCC(name));
            if (this.subList.Count > 0)
            {
                foreach (RIFFBase rb in this.subList)
                {
                    len += rb.Write(bw);
                }
            }
            else
            {
                //bw.Write(content);
                len = content.Length;
            }
            long sp = bw.BaseStream.Position;
            bw.BaseStream.Position -= len + 8;
            bw.Write(len + 4);
            bw.BaseStream.Position = sp;
            if ((len & 1) != 0)
            {
                bw.Write(false);
            }
            return (int)(bw.BaseStream.Position - p);
        }
        public override void Read(BinaryReader br, int len, int[] blockSizes = null)
        {
            Byte[] ba = new byte[len];
            br.Read(ba, 0, len);
            if (ba.Length > 8)
            {
                MemoryStream ms = new MemoryStream(ba);
                BinaryReader br1 = new BinaryReader(ms);
                this.subList = RiffConn.ReadRiff(this.Parent, br1, blockSizes);
                br1.Close();
                ms.Close();
            }
            else
            {
                this.content = ba;
            }
        }
    }
}
