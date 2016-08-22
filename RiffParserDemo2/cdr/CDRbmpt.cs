using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    public class CDRbmpt : RiffList
    {
        public CDRbmpt(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            base.Read(br, len, blockSizes);
            //this.Parent.BMPCount += this.subList.Count;
        }
    }
}
