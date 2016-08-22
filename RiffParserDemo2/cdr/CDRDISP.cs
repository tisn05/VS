using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace tisn.cdr
{
    public class CDRDISP:RiffChunk
    {
        public CDRDISP(CDRFile p) : base(p) { }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            byte[] tmp = br.ReadBytes(len);
            BinaryWriter bw = new BinaryWriter(new FileStream(this.tmpPath + "thumbnail.bmp", FileMode.Create));
            bw.Write(new char[] { 'B', 'M' });
            bw.Write(len + 10);
            bw.Write(0);
            bw.Write(tmp);
            bw.Close();
            //base.Read(br, len, blockSizes);
        }
    }
}
