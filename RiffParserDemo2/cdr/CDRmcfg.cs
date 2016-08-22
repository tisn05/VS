using System;
using System.Collections.Generic;
using System.Text;

namespace tisn.cdr
{
    public class CDRmcfg : RiffChunk
    {
        public CDRmcfg(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            long p = br.BaseStream.Position;

            int offset = 0x4;
            if (Parent.Version >= 13)
                offset = 0xc;
            if (Parent.Version == 7 || Parent.Version == 8)
                offset = 0x0;
            br.BaseStream.Position += offset;
            this.Parent.PageWidth = br.ReadInt32() / 10000;
            this.Parent.PageHeight = br.ReadInt32() / 10000;

            br.BaseStream.Position = p;

            base.Read(br, len, blockSizes);
        }
    }
}