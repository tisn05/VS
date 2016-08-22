using RiffParserDemo2;
using System;
using System.Collections.Generic;
using System.Text;
//using ICSharpCode.SharpZipLib.Tar;
using System.IO;

namespace tisn.cdr
{
    public class CDRReader
    {
        private RiffParser m_parser;
        public RiffParser Parser
        {
            get
            {
                return m_parser;
            }
        }
        private CDRFile m_cdrFile;
        public CDRFile CDRFile
        {
            get
            {
                return m_cdrFile;
            }
        }
        public CDRReader(RiffParser parser)
        {
            m_parser = parser;
            m_cdrFile = new CDRFile();
            m_cdrFile.list = new List<RIFFBase>();
        }
        public void Read()
        {
            CDRFile.verType = RiffParser.FromFourCC(m_parser.FileType);
            int length = Parser.DataSize;

            FileStream fs = Parser.Stream;
            BinaryReader br = new BinaryReader(fs);
            try
            {
                this.m_cdrFile.list = RiffConn.ReadRiff(this.m_cdrFile, br, m_parser.DataSize - 4);
            }
            catch
            {

            }
            finally
            {
                br.Close();
                fs.Close();
            }
        }
    }
}
