using System;
using System.Collections.Generic;
using System.Text;

namespace tisn.cdr
{
    public class CDRsumi : RiffChunk
    {
        public int SavedVersion = 0;
        public int SavedBuildID = 0;
        public int LanguageID = 0;
        public CDRsumi(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            base.Read(br, len, blockSizes);
            long p = br.BaseStream.Position;
            br.BaseStream.Position -= len;
            br.BaseStream.Position += 0x24;
            SavedVersion = br.ReadInt32() / 100;
            SavedBuildID = br.ReadInt32();
            if (len > 0x34)
            {
                br.ReadInt32();
                LanguageID = br.ReadInt32();
            }
            this.Parent.SVersion = SavedVersion;
            this.Parent.BuildID = SavedBuildID;
            br.BaseStream.Position = p;
        }
    }
}
