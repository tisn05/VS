using System;
using System.Reflection;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Acrobat;

namespace JSObjectControlCS
{
    public partial class JSObjectControlForm : Form
    {
        private AcroAVDoc g_AVDoc = null;

        public JSObjectControlForm()
        {
            InitializeComponent();
        }

        /* utility function to show open dialog */
        private String chooseFile(String filter)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = filter;
            dlg.CheckFileExists = true;
            dlg.InitialDirectory = Application.StartupPath;

            if (dlg.ShowDialog() == DialogResult.OK)
                return dlg.FileName;
            else
                return null;
        }

        /* open a pdf file in Acrobat, and check for validity */
        private void browseButton_Click(object sender, EventArgs e)
        {
            String filter = "PDF Files (*.pdf)|*.PDF|" +
              "All files (*.*)|*.*";
            String filename = chooseFile(filter);
            if (filename != null)
            {
                filenameTextBox.Text = filename;
                filenameTextBox_LostFocus(this, null);
            }
        }

        /* open data file, and then create individual forms for each row.
         * The open document must contain an template. You can use FormSample.pdf
         * and data.txt in the TestFiles directory.
         */
        private void processFormDataButton_Click(object sender, EventArgs e)
        {
            String filter = "Text Files (*.txt)|*.TXT|" +
              "All files (*.*)|*.*";
             String filename = chooseFile(filter);
             if (filename != null && g_AVDoc.IsValid())
             {
                 CAcroPDDoc pdDoc = (CAcroPDDoc)g_AVDoc.GetPDDoc();
                 Object jsObj = pdDoc.GetJSObject();

                 /*For each row in data.txt, as defined by the NUM_ROWS constant, we:
                
                     1. Spawn the form template.
                     2. Populate the form with the data from data.txt.
                     3. Save the PDF.
                  */
                Object newDoc = null;
                Object template = null;
                 bool formDataExists = true;
                 int currentIndex = 0;
                while(formDataExists)
                {
                    /*Acquire the form template and spawn a new page.
                     * 
                     * We could use the doc.templates object to get all templates
                     * in the doc and then spawn the first, but since we have a specific
                     * data file, we will look for a specific template.
                     */
                    object[] getTemplateParam = { "Templates:1" };

                    Type T = jsObj.GetType();
                    template = T.InvokeMember(
                        "getTemplate",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, jsObj, getTemplateParam);
                    
                    if(template == null)
                    {
                        MessageBox.Show("Failed to acquire form template.");
                       return;
                    }
                    
                    object[] spawnParam = { 1, false, false };

                    Type templateType = template.GetType();
                    templateType.InvokeMember(
                        "spawn",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, template, spawnParam);


                   /*Extract the page and populate the form with the data*/
                    object[] extractPagesParam = { 1 };
                    newDoc = T.InvokeMember(
                        "extractPages",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, jsObj, extractPagesParam);

                    object[] importTextDataParam = { filename, currentIndex };
                    double importError =(double) T.InvokeMember(
                        "importTextData",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, newDoc, importTextDataParam);
                        
                       if(importError == 0)
                    {
                        /* Save the new PDF to the output folder at the top level of the sample
                         * output folder has to exist, so create it first.
                         */
                        String newFileName = Application.StartupPath + "\\..\\..\\..\\output\\data" + (currentIndex + 1) + ".pdf";
                        Directory.CreateDirectory(Application.StartupPath + "\\..\\..\\..\\output");
                           object[] saveAsParam = { newFileName };
                        T.InvokeMember(
                            "saveAs",
                            BindingFlags.InvokeMethod |
                            BindingFlags.Public |
                            BindingFlags.Instance,
                            null, newDoc, saveAsParam);
                     }
                     else
                        formDataExists = false;

                object[] closeDocParam = { !formDataExists };
                T.InvokeMember(
                        "closeDoc",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, newDoc, closeDocParam);

                    currentIndex++;
                
                }
            }
        }

        /* Adds two watermarks: one using a pdf file and a the second text.
         * You can use the stamp.pdf file in the TestFiles directory.
        */
        private void addWatermarkButton_Click(object sender, EventArgs e)
        {
            String filter = "PDF Files (*.pdf)|*.PDF|" +
              "All files (*.*)|*.*";
            String filename = chooseFile(filter);
            if (filename != null && g_AVDoc.IsValid())
            {
                CAcroPDDoc pdDoc = (CAcroPDDoc)g_AVDoc.GetPDDoc();
                 //Acquire the Acrobat JavaScript Object interface from the PDDoc object
                Object jsObj= pdDoc.GetJSObject();

               /* Add a watermark from a file. 
                * See the readme for a discussion on InvokeMember.
                function prototype:
                  addWatermarkFromFile(cDIPath, nSourcePage, nStart, nEnd, bOnTop, bOnScreen, bOnPrint, nHorizAlign, nVertAlign, nHorizValue, nVertValue, bPercentage, nScale, bFixedPrint, nRotation, nOpacity)
               */
                object[] addFileWatermarkParam = { filename, 0, 0, 0, true, true, true, 0, 3, 10, -10, false, 0.4, false, 0, 0.7 };

                Type T = jsObj.GetType();
                T.InvokeMember(
                        "addWatermarkFromFile",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, jsObj, addFileWatermarkParam);

                //get current time and make a string from it
                System.DateTime currentTime = System.DateTime.Now;

                /* make a color object */
                Object colorObj = T.InvokeMember(
                        "color",
                        BindingFlags.GetProperty |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, jsObj, null);
                Type colorType = colorObj.GetType();
                Object blueColorObj = colorType.InvokeMember(
                        "blue",
                        BindingFlags.GetProperty |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, colorObj, null);

                /* Add a text watermark.
                ' function prototype:
                '   addWatermarkFromText(cText, nTextAlign, cFont, nFontSize, color, nStart, nEnd, bOnTop, bOnScreen, bOnPrint, nHorizAlign, nVertAlign, nHorizValue, nVertValue, bPercentage, nScale, bFixedPrint, nRotation, nOpacity)
                */
                 object[] addTextWatermarkParam = { currentTime.ToShortTimeString(), 1, "Helvetica", 100, blueColorObj, 0, 0, true, true, true, 0, 3, 20, -45, false, 1.0, false, 0, 0.7 };
                T.InvokeMember(
                        "addWatermarkFromText",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, jsObj, addTextWatermarkParam);              
            }
        }

        /* searches the specified pdf for the word in the searchTextBox, looping 
         through all matches until done or cancelled.
        */
        private void searchButton_Click(object sender, EventArgs e)
        {
            if (g_AVDoc.IsValid())
            {
                CAcroPDDoc pdDoc = (CAcroPDDoc)g_AVDoc.GetPDDoc();
                 //Acquire the Acrobat JavaScript Object interface from the PDDoc object
                Object jsObj= pdDoc.GetJSObject();
                Type T = jsObj.GetType();

                int nCount = 0;
                bool continueSearch = true;
		        currentStatus.Text = "Searching ... ";
			
			    // total number of pages
                double nPages = (double)T.InvokeMember(
                         "numPages",
                         BindingFlags.GetProperty |
                         BindingFlags.Public |
                         BindingFlags.Instance,
                         null, jsObj, null);
			
			    //Go through pages
			    for(int i = 0 ; i< nPages && continueSearch; i++)
				{
				    // check each word in a page
				    object[] getPageNumWordsParam = { i };
                    double nWords = (double)T.InvokeMember(
                        "getPageNumWords",
                        BindingFlags.InvokeMethod |
                        BindingFlags.Public |
                        BindingFlags.Instance,
                        null, jsObj, getPageNumWordsParam);

                    for (int j = 0; j < nWords && continueSearch; j++)
				    {
					    //get a word
                        object[] getPageNthWordParam = { i, j };
					    String word = (String)T.InvokeMember(
                            "getPageNthWord",
                            BindingFlags.InvokeMethod |
                            BindingFlags.Public |
                            BindingFlags.Instance,
                            null, jsObj, getPageNthWordParam);
    					
                        // compare the word with what the user wants
                        int result = String.Compare(word, searchTextBox.Text);
    						
					    // if same
					    if(result == 0)
                        {
						    nCount = nCount + 1;
                            currentStatus.Text = "word found at "+j;
						    T.InvokeMember(
                                "selectPageNthWord",
                                BindingFlags.InvokeMethod |
                                BindingFlags.Public |
                                BindingFlags.Instance,
                                null, jsObj, getPageNthWordParam);
    												
						    
                            if (MessageBox.Show( "Continue Search?", 
				                "", MessageBoxButtons.YesNo, MessageBoxIcon.Question ) == DialogResult.No)
                                continueSearch = false;
                        }
                    }
                    if (continueSearch == true)
                        currentStatus.Text = "Search Done";
                    else
                        currentStatus.Text = "User Cancelled Search";

                }
            }
        }

        /* validate file name, and if it exists, open it in Acrobat.
         * Then enable tasks.
        */
        private void filenameTextBox_LostFocus(object sender, EventArgs e)
        {
            String filename = filenameTextBox.Text;
            FileInfo fileinfo = new FileInfo(filename);
            if (fileinfo.Exists)
            {
                filenameTextBox.ForeColor = Color.Black;
                //assume this is a request to load a new doc or reopen the current one
                if (g_AVDoc != null)
                {
                    currentStatus.Text = "Closing Document...";
                    g_AVDoc.Close(0);
                }
                currentStatus.Text = "Opening Document...";
                g_AVDoc = new AcroAVDoc();
                g_AVDoc.Open(filename, "");
                currentStatus.Text = "Document Open";
                tasksPanel.Enabled = true;
                searchPanel.Enabled = true;
            }
            else
            {
                currentStatus.Text = "Document Doesn't Exist";
                filenameTextBox.ForeColor = Color.Red;
                tasksPanel.Enabled = false;
                searchPanel.Enabled = false;
            }
        }

        /* Enable search button if there is some text in the search text box,
         * else disable it.
        */
        void searchTextBox_TextChanged(object sender, EventArgs e)
        {
            if (searchTextBox.TextLength == 0)
                searchButton.Enabled = false;
            else
                searchButton.Enabled = true;
        }
    }
}