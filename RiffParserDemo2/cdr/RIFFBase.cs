using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace tisn.cdr
{
    public abstract class RIFFBase
    {
        public RiffType Type;
        public String name;
        public long postoin;
        public int length;
        public byte[] content;
        public abstract int Write(BinaryWriter bw);
        public abstract void Read(BinaryReader br, int len, int[] blockSizes = null);
        protected CDRFile Parent;
        public RIFFBase(CDRFile p)
        {
            this.Parent = p;
        }
    }
}
