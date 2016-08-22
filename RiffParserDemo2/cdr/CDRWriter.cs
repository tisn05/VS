using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace tisn.cdr
{
    public class CDRWriter
    {
        private CDRFile m_cdrFile;
        public void SetCDRFile(CDRFile f)
        {
            m_cdrFile = f;
        }
        public void Write(String cdrPath)
        {
            if (m_cdrFile == null)
            {
                return;
            }
            FileStream fs = File.Open(cdrPath, FileMode.OpenOrCreate);
            BinaryWriter bw = new BinaryWriter(fs);
            bw.Write(RiffParser.ToFourCC("RIFF"));
            bw.Write(0);
            bw.Write(RiffParser.ToFourCC(m_cdrFile.verType));
            foreach (RIFFBase rl in m_cdrFile.list)
            {
                rl.Write(bw);
            }
            int pos = (int)fs.Position;
            bw.Seek(4, SeekOrigin.Begin);
            bw.Write(pos - 8);
            bw.Close();
            fs.Close();
        }
    }
}
