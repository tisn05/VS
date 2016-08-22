namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Text;

    public class blockinfoArray
    {
        public object[] infoblockinfoarray(blockinfoModel infomodel, int count)
        {
            object[] objArray = new object[count];
            blockinfobyteModel model = new blockinfobyteModel {
                bytemin_version = BitConverter.GetBytes(infomodel.min_version1),
                bytemin_tip = Encoding.Unicode.GetBytes(infomodel.min_tip2),
                bytecompress_type = BitConverter.GetBytes(infomodel.compress_type3),
                bytecompress_block_size = BitConverter.GetBytes(infomodel.compress_block_size4),
                bytedatatype = BitConverter.GetBytes(infomodel.datatype5),
                bytelocale = BitConverter.GetBytes(infomodel.locale6),
                byteencoding = BitConverter.GetBytes(infomodel.encoding7),
                bytechapter_index = BitConverter.GetBytes(infomodel.chapter_index8),
                bytechapter_level = BitConverter.GetBytes(infomodel.chapter_level9),
                bytechapter_name = Encoding.Unicode.GetBytes(infomodel.chapter_name10),
                bytefile_list_offset11 = BitConverter.GetBytes(infomodel.file_list_offset11),
                bytechapter_content_decompress_lengt12 = BitConverter.GetBytes(infomodel.chapter_content_decompress_length12),
                bytechapter_file_size13 = BitConverter.GetBytes(infomodel.chapter_file_size13)
            };
            objArray[0] = model.bytemin_version;
            objArray[1] = model.bytemin_tip;
            objArray[2] = model.bytecompress_type;
            objArray[3] = model.bytecompress_block_size;
            objArray[4] = model.bytedatatype;
            objArray[5] = model.bytelocale;
            objArray[6] = model.byteencoding;
            objArray[7] = model.bytechapter_index;
            objArray[8] = model.bytechapter_level;
            objArray[9] = model.bytechapter_name;
            objArray[10] = model.bytefile_list_offset11;
            objArray[11] = model.bytechapter_content_decompress_lengt12;
            objArray[12] = model.bytechapter_file_size13;
            return objArray;
        }
    }
}

