using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;

namespace MatrixVisualization
{
  public partial class Form1 : Form
  {
    Bitmap image;
    public Form1()
    {
      InitializeComponent();
    }

    private void loadToolStripMenuItem_Click(object sender, EventArgs e)
    {
      OpenFileDialog openFileDialog = new OpenFileDialog();

      DialogResult dialogResult = openFileDialog.ShowDialog();
      String textFromFile = null;
      if (dialogResult == DialogResult.OK)
      {
        String path = openFileDialog.FileName;
        using (FileStream fstream = File.OpenRead(path))
        {
          // выделяем массив для считывания данных из файла
          byte[] buffer = new byte[fstream.Length];
          fstream.Read(buffer, 0, buffer.Length);
          textFromFile = Encoding.Default.GetString(buffer);
        }
      }

      if (textFromFile != null)
      {
        String[] lineParsed = textFromFile.Split('\n');
        int[][] data = new int[int.Parse(lineParsed[0])][];
        for(int i = 0; i < data.Length; i++)
        {
          String[] line = lineParsed[i+1].Split(' ');
          data[i] = new int[2];
          data[i][0] = int.Parse(line[0]);
          data[i][1] = int.Parse(line[1]);
        }

        image = new Bitmap(data[0][0]+1, data[0][1]+1);
        for(int i = 1; i < data.Length; i++)
        {
          image.SetPixel(data[i][0], data[i][1], Color.Black);
        }
        pictureBox1.Image = image;
      }
     
    }
  }
}
