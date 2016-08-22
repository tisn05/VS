using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    /// <summary>
    /// 轮廓
    /// </summary>
    public class CDRoutl : RiffChunk
    {
        public CDRoutl(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            long p = br.BaseStream.Position;

            int OutlineID = br.ReadInt32();
            Debug.WriteLine(String.Format("Outline ID:{0:d}", OutlineID));

            int lt = 0x4, ct = 0x6, jt = 0x8, lw = 0xc, st = 0x10, ang = 0x14, varo = 0x1c, lc = 0x4c, dash = 0x68, arrw = 0x80;

            if (Parent.Version >= 13)
            {
                int flag = br.ReadInt32();
                int off = 0;
                if (flag == 5)
                    off = 107;
                else if (Parent.Version >= 16)
                    off = 51;
                lt = 0x18 + off;
                ct = 0x1a + off;
                jt = 0x1c + off;
                lw = 0x1e + off;
                st = 0x22 + off;
                ang = 0x24 + off;
                varo = 0x28 + off;
                lc = 0x58 + off;
                dash = 0x74 + off;
                arrw = 0x8a + off;
            }
            else if (Parent.Version < 6)
            {
                lw = 0xa;
                st = 0xc;
                ang = 0xe;
                lc = 0x10;
                dash = 0x26;
            }
            br.BaseStream.Position = p + lt;
            short ltype = br.ReadInt16();
            String ltxt = "Non-scalable";
            if ((ltype & 0x20) == 0x20)
            {
                ltxt = "Scalable";
            }
            if ((ltype & 0x10) == 0x10)
            {
                ltxt += ", Behind fill";
            }
            if ((ltype & 0x80) == 0x80)
            {
                ltxt += ", Share Attrs";
            }
            if ((ltype & 0x4) == 0x4)
            {
                ltxt += ", Dashed";
            }

            Debug.WriteLine(String.Format("Line Type {0:d} {1:s}", ltype, ltxt));
            br.BaseStream.Position = p + ct;
            Debug.WriteLine(String.Format("Caps Type {0:d}", br.ReadByte()));
            br.BaseStream.Position = p + jt;
            Debug.WriteLine(String.Format("Join Type {0:d}", br.ReadByte()));
            br.BaseStream.Position = p + lw;
            Debug.WriteLine(String.Format("LineWidth {0:f}", br.ReadInt32() / 10000.0));

            br.BaseStream.Position = p;
            base.Read(br, len, blockSizes);
        }
    }
}
