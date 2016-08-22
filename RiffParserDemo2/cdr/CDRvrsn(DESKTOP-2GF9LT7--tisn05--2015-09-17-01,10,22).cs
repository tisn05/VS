using System;
using System.Collections.Generic;
using System.Text;

namespace tisn.cdr
{
    public class CDRvrsn : RiffChunk
    {
        private short m_version = 0;
        public short Version
        {
            get
            {
                return m_version;
            }
        }
        public CDRvrsn(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            if (len == 16)
            {
                br.BaseStream.Position += 8;
                this.Parent.Version = (short)(br.ReadInt16() / 100);
                br.BaseStream.Position += 6;
                return;
            }
            this.Parent.Version = (short)(br.ReadInt16() / 100);
        }
        public override int Write(System.IO.BinaryWriter bw)
        {
            if (this.content != null)
            {
                return base.Write(bw);
            }
            long p = bw.BaseStream.Position;
            bw.Write(RiffParser.ToFourCC(name));
            bw.Write(2);
            bw.Write((short)(m_version * 100));
            return (int)(bw.BaseStream.Position - p);
        }
    }
}
