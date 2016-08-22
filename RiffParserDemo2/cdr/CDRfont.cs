using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    /// <summary>
    /// 字体
    /// </summary>
    public class CDRfont : RiffChunk
    {

        public int FontID = 0;
        public String FontName = null;
        public CDRfont(CDRFile p)
            : base(p)
        {

        }
        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            long p = br.BaseStream.Position;

            FontID = br.ReadInt16();
            Debug.WriteLine(String.Format("字体ID:{0:d}", FontID));
            int enc = br.ReadInt16();
            String enctxt = "Unknown";
            if (CDRFile.Font_Charsets.ContainsKey(enc))
            {
                enctxt = CDRFile.Font_Charsets[enc];
            }
            Debug.WriteLine(String.Format("字体编码:{0:s}", enctxt));
            int shift = 0;
            if (Parent.Version > 5)
            {
                shift = 2;
            }
            byte[] Flags = br.ReadBytes(14);
            byte[] fn = br.ReadBytes(34);
            FontName = Encoding.Default.GetString(fn);
            if (Parent.Version > 11)
            {
                FontName = Encoding.Unicode.GetString(fn);
            }
            Debug.WriteLine(String.Format("字体名称:{0:s}", FontName));
            tisn.cdr.CDRFile.Fonts fs = new tisn.cdr.CDRFile.Fonts();
            fs.name = FontName;
            fs.encoding = enctxt;
            Parent.fonts.Add(fs);

            br.BaseStream.Position = p;
            base.Read(br, len, blockSizes);
        }
    }
}
