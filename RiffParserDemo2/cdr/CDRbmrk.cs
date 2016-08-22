using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    /// <summary>
    /// 视图样式
    /// </summary>
    public class CDRbmrk:RiffChunk
    {
        public CDRbmrk(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            base.Read(br, len, blockSizes);
            Parent.ViewCount++;
        }
    }
}
