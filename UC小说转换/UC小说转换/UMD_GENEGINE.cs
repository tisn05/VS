namespace Ikari
{
    using ICSharpCode.SharpZipLib.Zip.Compression;
    using System;
    using System.Collections;
    using System.IO;
    using System.Reflection;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Windows.Forms;

    public class UMD_GENEGINE
    {
        private const int A_32K_BYTE = 0x8000;
        private byte ACTUAL_WIDTH_S60_HORI = 0xcc;
        private byte ACTUAL_WIDTH_S60_VERT = 0xac;
        private byte ACTUAL_WIDTH_SP = 0xa6;
        private const uint BASE_REFN_CHAP_OFF = 0x3000;
        private const uint BASE_REFN_CHAP_STR = 0x4000;
        private const uint BASE_REFN_CONTENT = 0x2000;
        private const uint BASE_REFN_COVER = 0x1000;
        private const uint BASE_REFN_PAGE_OFFSET = 0x7000;
        private const string BEYOND_END_FLAG = "\0";
        private const int BYTE_LEN = 1;
        private const byte COVER_TYPE_BMP = 0;
        private const byte COVER_TYPE_GIF = 2;
        private const byte COVER_TYPE_JPG = 1;
        private const int CURR_VERSION = 1;
        private const short DCTS_CMD_ID_AUTHOR = 3;
        private const short DCTS_CMD_ID_CDS_KEY = 240;
        private const short DCTS_CMD_ID_CHAP_OFF = 0x83;
        private const short DCTS_CMD_ID_CHAP_STR = 0x84;
        private const short DCTS_CMD_ID_CONTENT_ID = 10;
        private const short DCTS_CMD_ID_COVER_PAGE = 130;
        private const short DCTS_CMD_ID_DAY = 6;
        private const short DCTS_CMD_ID_FILE_LENGTH = 11;
        private const short DCTS_CMD_ID_FIXED_LEN = 12;
        private const short DCTS_CMD_ID_GENDER = 7;
        private const short DCTS_CMD_ID_LICENSE_KEY = 0xf1;
        private const short DCTS_CMD_ID_MONTH = 5;
        private const short DCTS_CMD_ID_PAGE_OFFSET = 0x87;
        private const short DCTS_CMD_ID_PUBLISHER = 8;
        private const short DCTS_CMD_ID_REF_CONTENT = 0x81;
        private const short DCTS_CMD_ID_TITLE = 2;
        private const short DCTS_CMD_ID_VENDOR = 9;
        private const short DCTS_CMD_ID_VERSION = 1;
        private const short DCTS_CMD_ID_YEAR = 4;
        private const byte FIXED_LINE_PER_PAGE_S60 = 50;
        private const byte FIXED_LINE_PER_PAGE_SP = 0x19;
        private string iAuthor;
        private byte[] ibContent;
        private int[] iChapOff;
        private ArrayList iChapStr = new ArrayList();
        private ArrayList iChapter = new ArrayList();
        private int iCID;
        private string iContent;
        private string iCoverFile;
        private string iDay;
        private string iGender;
        private string iMonth;
        private const int INT_LEN = 4;
        private short iPGKSeed = 0;
        private string iPublisher;
        private string iSaveTo;
        private string iTitle;
        private int iTotalen;
        private string iVendor;
        private ArrayList iWidthData_S60 = new ArrayList();
        private ArrayList iWidthData_SP = new ArrayList();
        private string iYear;
        private byte[][] iZippedSeg;
        private const string KEY_CODE_TAB = "\t";
        private const byte S60_FONT_SIZE_BIG = 0x10;
        private const byte S60_FONT_SIZE_SMALL = 12;
        private const byte SEREIS60_FONTS_COUNT = 2;
        private const int SHORT_LEN = 2;
        private const byte SP_FONT_SIZE_10 = 10;
        private const byte SP_FONT_SIZE_MAX = 0x10;
        private const byte SP_FONT_SIZE_MIN = 6;
        private const string SYMBIAN_RETURN = "u2029";
        private const string SYMBIAN_SPACE = " ";
        private const byte UMD_DCTD_HEAD_LEN = 9;
        private const byte UMD_DCTS_HEAD_LEN = 5;
        private const int UMD_FREE_CID_MIN = 0x5f5e100;
        private const int UMD_FREE_PGK_SEED_MIN = 0x400;
        private const int UMD_LICENSEKEY_LEN = 0x10;
        private const int UMD_MAX_BOOKMARK_STR_LEN = 40;
        private const byte UMD_PLATFORM_ID_NONE = 0;
        private const byte UMD_PLATFORM_ID_S60 = 1;
        private const byte UMD_PLATFORM_ID_SP = 5;
        private const int UMD_SEGMENT_LENGTH = 0x8000;
        private const byte VER_PKG_LEN = 3;
        private const string WINDOWS_RETURN = "\r\n";
        private const int ZIP_LEVEL = 9;

        private byte CharWidth_S60(string Char, byte pFontSize)
        {
            ushort num = Char[0];
            for (int i = 0; i < this.iWidthData_S60.Count; i++)
            {
                SWidthData data = (SWidthData) this.iWidthData_S60[i];
                if (((data.FontSize == pFontSize) && (num >= data.rngFrom)) && (num <= data.rngTo))
                {
                    if (data.vCount == 1)
                    {
                        return data.Value[0];
                    }
                    return data.Value[num - data.rngFrom];
                }
            }
            return pFontSize;
        }

        private byte CharWidth_SP(string Char, byte pFontSize)
        {
            ushort num = Char[0];
            for (int i = 0; i < this.iWidthData_SP.Count; i++)
            {
                SWidthData data = (SWidthData) this.iWidthData_SP[i];
                if (((data.FontSize == pFontSize) && (num >= data.rngFrom)) && (num <= data.rngTo))
                {
                    if (data.vCount == 1)
                    {
                        return data.Value[0];
                    }
                    return data.Value[num - data.rngFrom];
                }
            }
            return pFontSize;
        }

        private bool GetPageOffsetS60(byte size, uint actual_width, ref System.Windows.Controls.ProgressBar pbar, out uint[] result)
        {
            ArrayList pPageoff = new ArrayList();
            if ((size != 0x10) && (size != 12))
            {
                result = new uint[0];
                return false;
            }
            this.GetWidthData_S60();
            pPageoff.Add(0);
            int num = (int)pbar.Value;
            while (((int) pPageoff[pPageoff.Count - 1]) < this.iContent.Length)
            {
                this.ParseOnePage((uint) (pPageoff.Count - 1), size, actual_width, ref pPageoff, 1);
                pbar.Value = num + ((int) ((uint) pPageoff[pPageoff.Count - 1]));
            }
            result = new uint[pPageoff.Count];
            for (int i = 0; i < pPageoff.Count; i++)
            {
                result[i] = ((uint) pPageoff[i]) * 2;
            }
            return true;
        }

        private bool GetPageOffsetSP(byte size, uint actual_width, ref System.Windows.Controls.ProgressBar pbar, out uint[] result)
        {
            ArrayList pPageoff = new ArrayList();
            if ((size < 6) || (size > 0x10))
            {
                result = new uint[0];
                return false;
            }
            this.GetWidthData_SP();
            pPageoff.Add(0);
            int num = (int)pbar.Value;
            while (((uint) pPageoff[pPageoff.Count - 1]) < this.iContent.Length)
            {
                this.ParseOnePage((uint) (pPageoff.Count - 1), size, actual_width, ref pPageoff, 5);
                if ((num + ((int) ((uint) pPageoff[pPageoff.Count - 1]))) < pbar.Maximum)
                {
                    pbar.Value = num + ((int) ((uint) pPageoff[pPageoff.Count - 1]));
                }
            }
            result = new uint[pPageoff.Count];
            for (int i = 0; i < pPageoff.Count; i++)
            {
                result[i] = (uint) pPageoff[i];
            }
            return true;
        }

        private void GetWidthData_S60()
        {
            this.iWidthData_S60.Clear();
            for (int i = 0; i < 2; i++)
            {
                string path = "";
                switch (i)
                {
                    case 0:
                        path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase.Remove(0, 8)) + @"\FontWidthData\S60CHS.S16.wdt";
                        break;

                    case 1:
                        path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase.Remove(0, 8)) + @"\FontWidthData\S60CHS.S12.wdt";
                        break;
                }
                if (File.Exists(path))
                {
                    FileStream input = new FileStream(path, FileMode.Open, FileAccess.Read);
                    BinaryReader reader = new BinaryReader(input);
                    while (reader.BaseStream.Position < reader.BaseStream.Length)
                    {
                        ushort num2 = reader.ReadUInt16();
                        ushort num3 = reader.ReadUInt16();
                        uint num4 = reader.ReadUInt16();
                        byte[] buffer = new byte[num4];
                        for (uint j = 0; j < num4; j++)
                        {
                            buffer[j] = reader.ReadByte();
                        }
                        SWidthData data = new SWidthData();
                        switch (i)
                        {
                            case 0:
                                data.FontSize = 0x10;
                                break;

                            case 1:
                                data.FontSize = 12;
                                break;
                        }
                        data.rngFrom = num2;
                        data.rngTo = num3;
                        data.vCount = num4;
                        data.Value = buffer;
                        this.iWidthData_S60.Add(data);
                    }
                    reader.Close();
                    input.Close();
                }
            }
        }

        private void GetWidthData_SP()
        {
            this.iWidthData_SP.Clear();
            for (int i = 0x10; i < 0x11; i++)
            {
                string path = (Assembly.GetExecutingAssembly().CodeBase.Remove(0, 8) + @"\FontWidthData\sunfon.s") + i.ToString() + ".wdt";
                if (File.Exists(path))
                {
                    FileStream input = new FileStream(path, FileMode.Open, FileAccess.Read);
                    BinaryReader reader = new BinaryReader(input);
                    while (reader.BaseStream.Position < reader.BaseStream.Length)
                    {
                        ushort num2 = reader.ReadUInt16();
                        ushort num3 = reader.ReadUInt16();
                        uint num4 = reader.ReadUInt16();
                        byte[] buffer = new byte[num4];
                        for (uint j = 0; j < num4; j++)
                        {
                            buffer[j] = reader.ReadByte();
                        }
                        SWidthData data = new SWidthData();
                        data.FontSize = (byte) i;
                        data.rngFrom = num2;
                        data.rngTo = num3;
                        data.vCount = num4;
                        data.Value = buffer;
                        this.iWidthData_SP.Add(data);
                    }
                    reader.Close();
                    input.Close();
                }
            }
        }

        public bool Initialize(string aTitle, string aAuthor, string aYear, string aMonth, string aDay, string aGender, string aPublisher, string aVendor, string aCoverFile, int aContentID, string aSaveTo, ref ArrayList aChapter, ref ArrayList aChapStr, out string aResult)
        {
            aResult = "true";
            if (aTitle.Length <= 0)
            {
                aResult = "标题不能为空";
                return false;
            }
            if (aAuthor.Length <= 0)
            {
                aResult = "作者不能为空";
                return false;
            }
            if (((aYear.Length > 4) || (aMonth.Length > 2)) || (aDay.Length > 2))
            {
                aResult = "日期非法";
                return false;
            }
            if (aChapter.Count <= 0)
            {
                aResult = "内容数量不能小于0";
                return false;
            }
            if (aChapter.Count != aChapStr.Count)
            {
                aResult = "章节标题数量和章节内容数量不符";
                return false;
            }
            if (!Directory.Exists(aSaveTo))
            {
                aResult = "保存文件的路径不存在";
                return false;
            }
            this.iTitle = aTitle;
            this.iAuthor = aAuthor;
            this.iYear = aYear;
            this.iMonth = aMonth;
            this.iDay = aDay;
            this.iGender = aGender;
            this.iPublisher = aPublisher;
            this.iVendor = aVendor;
            this.iCID = aContentID;
            this.iCoverFile = aCoverFile;
            this.iSaveTo = aSaveTo;
            //File.Copy(this.iCoverFile + ".tmp", this.iSaveTo + @"\" + this.iTitle + ".jpg");
            for (int i = 0; i < aChapter.Count; i++)
            {
                this.iChapter.Add(((string) aChapter[i]) + "\u2029");
                this.iChapStr.Add((string) aChapStr[i]);
            }
            this.iChapOff = new int[this.iChapter.Count];
            return true;
        }

        public bool Make(ref System.Windows.Controls.ProgressBar pbar, out string result)
        {
            uint[] numArray2;
            this.Prepare();
            Random random = new Random();
            uint num = 0;
            if (this.iSaveTo.EndsWith(@"\"))
            {
                this.iSaveTo.Remove(this.iSaveTo.Length - 1, 1);
            }
            string path = this.iSaveTo + @"\" + this.iTitle + ".umd";
            if (File.Exists(path))
            {
                if (MessageBox.Show("您要保存的文件已经存在，是否覆盖？", "mBook制作工具 版本:1.0", MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes)
                {
                    result = "生成文件操作被用户取消";
                    return false;
                }
                File.Delete(path);
            }
            FileStream output = new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write);
            BinaryWriter writer = new BinaryWriter(output);
            byte num1 = Encoding.Unicode.GetBytes((string) this.iChapter[0])[0];
            writer.Write((uint) 0xde9a9b89);
            writer.Write('#');
            writer.Write((short) 1);
            writer.Write((byte) 0);
            writer.Write((byte) 8);
            writer.Write((byte) 1);
            writer.Write(this.iPGKSeed);
            writer.Write('#');
            writer.Write((short) 2);
            writer.Write((byte) 0);
            writer.Write((byte) (5 + (this.iTitle.Length * 2)));
            writer.Write(Encoding.Unicode.GetBytes(this.iTitle));
            writer.Write('#');
            writer.Write((short) 3);
            writer.Write((byte) 0);
            writer.Write((byte) (5 + (this.iAuthor.Length * 2)));
            writer.Write(Encoding.Unicode.GetBytes(this.iAuthor));
            if (this.iYear.Length > 0)
            {
                writer.Write('#');
                writer.Write((short) 4);
                writer.Write((byte) 0);
                writer.Write((byte) (5 + (this.iYear.Length * 2)));
                writer.Write(Encoding.Unicode.GetBytes(this.iYear));
            }
            if (this.iMonth.Length > 0)
            {
                writer.Write('#');
                writer.Write((short) 5);
                writer.Write((byte) 0);
                writer.Write((byte) (5 + (this.iMonth.Length * 2)));
                writer.Write(Encoding.Unicode.GetBytes(this.iMonth));
            }
            if (this.iDay.Length > 0)
            {
                writer.Write('#');
                writer.Write((short) 6);
                writer.Write((byte) 0);
                writer.Write((byte) (5 + (this.iDay.Length * 2)));
                writer.Write(Encoding.Unicode.GetBytes(this.iDay));
            }
            if (this.iGender.Length > 0)
            {
                writer.Write('#');
                writer.Write((short) 7);
                writer.Write((byte) 0);
                writer.Write((byte) (5 + (this.iGender.Length * 2)));
                writer.Write(Encoding.Unicode.GetBytes(this.iGender));
            }
            if (this.iPublisher.Length > 0)
            {
                writer.Write('#');
                writer.Write((short) 8);
                writer.Write((byte) 0);
                writer.Write((byte) (5 + (this.iPublisher.Length * 2)));
                writer.Write(Encoding.Unicode.GetBytes(this.iPublisher));
            }
            if (this.iVendor.Length > 0)
            {
                writer.Write('#');
                writer.Write((short) 9);
                writer.Write((byte) 0);
                writer.Write((byte) (5 + (this.iVendor.Length * 2)));
                writer.Write(Encoding.Unicode.GetBytes(this.iVendor));
            }
            writer.Write('#');
            writer.Write((short) 11);
            writer.Write((byte) 0);
            writer.Write((byte) 9);
            writer.Write(this.ibContent.Length);
            num = (uint) (0x3000L + random.Next(0xfff));
            writer.Write('#');
            writer.Write((short) 0x83);
            writer.Write((byte) 1);
            writer.Write((byte) 9);
            writer.Write(num);
            writer.Write('$');
            writer.Write(num);
            writer.Write((uint) (9 + (this.iChapOff.Length * 4)));
            foreach (int num2 in this.iChapOff)
            {
                writer.Write(num2);
            }
            num = (uint) (0x4000L + random.Next(0xfff));
            writer.Write('#');
            writer.Write((short) 0x84);
            writer.Write((byte) 1);
            writer.Write((byte) 9);
            writer.Write(num);
            int num3 = 0;
            foreach (object obj2 in this.iChapStr)
            {
                num3 += (((string) obj2).Length * 2) + 1;
            }
            writer.Write('$');
            writer.Write(num);
            writer.Write((uint) (9 + num3));
            foreach (object obj3 in this.iChapStr)
            {
                writer.Write((byte) (((string) obj3).Length * 2));
                writer.Write(Encoding.Unicode.GetBytes((string) obj3));
            }
            int num4 = 0;
            int num5 = 0;
            uint[] numArray = new uint[this.iZippedSeg.Length];
            if (this.iZippedSeg.Length > 1)
            {
                num4 = random.Next(0, this.iZippedSeg.Length - 1);
                num5 = random.Next(0, this.iZippedSeg.Length - 1);
            }
            for (int i = 0; i < this.iZippedSeg.Length; i++)
            {
                numArray[i] = (uint) (random.Next(1, 0xfffffff) * -1);
                writer.Write('$');
                writer.Write(numArray[i]);
                writer.Write((uint) (9 + this.iZippedSeg[i].Length));
                writer.Write(this.iZippedSeg[i]);
                if (i == num4)
                {
                    writer.Write('#');
                    writer.Write((short) 0xf1);
                    writer.Write((byte) 0);
                    writer.Write((byte) 0x15);
                    writer.Write(Encoding.ASCII.GetBytes("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"));
                }
                if (i == num5)
                {
                    writer.Write('#');
                    writer.Write((short) 10);
                    writer.Write((byte) 0);
                    writer.Write((byte) 9);
                    writer.Write(this.iCID);
                }
            }
            num = (uint) (0x2000L + random.Next(0xfff));
            writer.Write('#');
            writer.Write((short) 0x81);
            writer.Write((byte) 1);
            writer.Write((byte) 9);
            writer.Write(num);
            writer.Write('$');
            writer.Write(num);
            writer.Write((uint) (9 + (numArray.Length * 4)));
            foreach (uint num7 in numArray)
            {
                writer.Write(num7);
            }
            if (File.Exists(this.iCoverFile))
            {
                FileStream stream2 = new FileStream(this.iCoverFile + ".tmp", FileMode.Open);
                byte[] buffer = new byte[stream2.Length];
                stream2.Read(buffer, 0, (int) stream2.Length);
                stream2.Close();
                num = (uint) (0x1000L + random.Next(0xfff));
                writer.Write('#');
                writer.Write((short) 130);
                writer.Write((byte) 1);
                writer.Write((byte) 10);
                writer.Write((byte) 1);
                writer.Write(num);
                writer.Write('$');
                writer.Write(num);
                writer.Write((uint) (9 + buffer.Length));
                writer.Write(buffer);
            }
            pbar.Maximum = this.iContent.Length * 5;
            pbar.Value = 0;
            this.GetPageOffsetS60(0x10, this.ACTUAL_WIDTH_S60_HORI, ref pbar, out numArray2);
            this.WritePageOffset(0x10, (byte) (this.ACTUAL_WIDTH_S60_HORI + 4), ref numArray2, ref writer, 1);
            numArray2.Initialize();
            this.GetPageOffsetS60(0x10, this.ACTUAL_WIDTH_S60_VERT, ref pbar, out numArray2);
            this.WritePageOffset(0x10, (byte) (this.ACTUAL_WIDTH_S60_VERT + 4), ref numArray2, ref writer, 1);
            numArray2.Initialize();
            this.GetPageOffsetS60(12, this.ACTUAL_WIDTH_S60_HORI, ref pbar, out numArray2);
            this.WritePageOffset(12, (byte) (this.ACTUAL_WIDTH_S60_HORI + 4), ref numArray2, ref writer, 1);
            numArray2.Initialize();
            this.GetPageOffsetS60(12, this.ACTUAL_WIDTH_S60_VERT, ref pbar, out numArray2);
            this.WritePageOffset(12, (byte) (this.ACTUAL_WIDTH_S60_VERT + 4), ref numArray2, ref writer, 1);
            numArray2.Initialize();
            this.GetPageOffsetSP(10, this.ACTUAL_WIDTH_SP, ref pbar, out numArray2);
            this.WritePageOffset(10, this.ACTUAL_WIDTH_SP, ref numArray2, ref writer, 5);
            writer.Write('#');
            writer.Write((short) 12);
            writer.Write((byte) 1);
            writer.Write((byte) 9);
            writer.Write((uint) (((uint) writer.BaseStream.Position) + 4));
            writer.Close();
            output.Close();
            result = "true";
            return true;
        }

        private void ParseOnePage(uint pPageNumber, byte pFontSize, uint pScreenWidth, ref ArrayList pPageoff, byte PID)
        {
            if (pPageNumber < pPageoff.Count)
            {
                string str = "";
                int num = 0;
                uint num2 = (uint) pPageoff[(int) pPageNumber];
                ArrayList list = new ArrayList();
                byte num3 = 0;
                if (PID == 1)
                {
                    num3 = 50;
                }
                if (PID == 5)
                {
                    num3 = 0x19;
                }
                for (byte i = 0; i < num3; i = (byte) (i + 1))
                {
                    str = str.Remove(0, str.Length);
                    string str2 = "";
                    byte num5 = 0;
                Label_0061:
                    if (num2 < this.iContent.Length)
                    {
                        str2 = this.iContent.Substring((int) num2, 1);
                    }
                    else
                    {
                        str2 = "\0";
                    }
                    switch (str2)
                    {
                        case "\t":
                        case "\0":
                            str2 = " ";
                            break;
                    }
                    byte num6 = this.CharWidth_S60(str2, pFontSize);
                    if (str2 == "\u2029")
                    {
                        num6 = 0;
                    }
                    if ((num6 + num5) <= pScreenWidth)
                    {
                        num5 = (byte) (num5 + num6);
                        num2++;
                        if (!(str2 == "\u2029"))
                        {
                            str = str + str2;
                            goto Label_0061;
                        }
                    }
                    if (str2 != "\u2029")
                    {
                        list.Add(str.Length);
                    }
                    else
                    {
                        list.Add(str.Length + 1);
                    }
                    num += (int) list[i];
                    if (i == ((byte) (num3 - 1)))
                    {
                        if ((num2 < this.iContent.Length) && (num2 > ((uint) pPageoff[pPageoff.Count - 1])))
                        {
                            pPageoff.Add(((uint) pPageoff[pPageoff.Count - 1]) + ((uint) num));
                        }
                        if (num2 >= this.iContent.Length)
                        {
                            pPageoff.Add((uint) this.iContent.Length);
                        }
                    }
                }
            }
        }

        private void Prepare()
        {
            Random random = new Random();
            this.iPGKSeed = (short) (random.Next(0x401, 0x7fff) % 0xffff);
            this.iTotalen = 0;
            for (int i = 0; i < this.iChapter.Count; i++)
            {
                this.iChapter[i] = ((string) this.iChapter[i]).Replace("\r\n", "\u2029");
                this.iContent = this.iContent + ((string) this.iChapter[i]);
                this.iChapOff[i] = this.iTotalen;
                this.iTotalen += ((string) this.iChapter[i]).Length * 2;
            }
            this.ibContent = new byte[this.iTotalen];
            int index = 0;
            for (int j = 0; j < this.iChapter.Count; j++)
            {
                byte[] bytes = new byte[((string) this.iChapter[j]).Length * 2];
                bytes = Encoding.Unicode.GetBytes((string) this.iChapter[j]);
                bytes.CopyTo(this.ibContent, index);
                index += bytes.Length;
            }
            int num4 = 0;
            if ((this.iTotalen % 0x8000) == 0)
            {
                num4 = this.iTotalen / 0x8000;
            }
            else
            {
                num4 = (this.iTotalen / 0x8000) + 1;
            }
            this.iZippedSeg = new byte[num4][];
            byte[] input = new byte[0x8000];
            int num5 = 0;
            int num6 = 0;
            for (int k = 0; k < this.ibContent.Length; k++)
            {
                input[num5] = this.ibContent[k];
                if ((num5 == 0x7fff) || (k == (this.ibContent.Length - 1)))
                {
                    byte[] output = new byte[0x8000];
                    num5 = 0;
                    Deflater deflater = new Deflater(Deflater.BEST_COMPRESSION, false);
                    if (deflater.IsNeedingInput)
                    {
                        deflater.SetInput(input, 0, input.Length);
                    }
                    deflater.Finish();
                    deflater.Deflate(output);
                    this.iZippedSeg[num6] = new byte[deflater.TotalOut];
                    Deflater deflater2 = new Deflater(Deflater.BEST_COMPRESSION, false);
                    if (deflater2.IsNeedingInput)
                    {
                        deflater2.SetInput(input, 0, input.Length);
                    }
                    deflater2.Finish();
                    deflater2.Deflate(this.iZippedSeg[num6]);
                    num6++;
                    input = Encoding.Unicode.GetBytes("");
                    input = new byte[0x8000];
                }
                else
                {
                    num5++;
                }
            }
        }

        private void WritePageOffset(byte fontSize, byte screenWidth, ref uint[] data, ref BinaryWriter writer, byte PID)
        {
            Random random = new Random();
            uint num = (uint) (0x7000L + random.Next(0xfff));
            writer.Write('#');
            writer.Write((short) 0x87);
            writer.Write(PID);
            writer.Write((byte) 11);
            writer.Write(fontSize);
            writer.Write(screenWidth);
            writer.Write(num);
            writer.Write('$');
            writer.Write(num);
            writer.Write((uint) (9 + (data.Length * 4)));
            foreach (uint num2 in data)
            {
                writer.Write(num2);
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct SWidthData
        {
            public byte FontSize;
            public ushort rngFrom;
            public ushort rngTo;
            public uint vCount;
            public byte[] Value;
        }
    }
}