using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

//__________ External libraries
using CsvHelper;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Legends;
using OxyPlot.Series;
using MathNet.Numerics;


namespace _2D_Contour_Plotter
{
    public partial class main_form : Form
    {
        Dictionary<string, List<double>> data_input = new Dictionary<string, List<double>>();
        bool is_data_loaded = false;

        public main_form()
        {
            InitializeComponent();
        }

        private void main_form_Load(object sender, EventArgs e)
        {
            comboBox_contourlevel.SelectedIndex = 3;
        }

        private void button_importcsv_Click(object sender, EventArgs e)
        {

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
                openFileDialog.Title = "Select a CSV file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    // Get the selected file path
                    string file_loc = openFileDialog.FileName;

                    is_data_loaded = false;
                    data_input = new Dictionary<string, List<double>>();
                    data_input = ReadCsv(file_loc);
                    comboBox_dataheader.Items.Clear();

                    if (IsValidContourData(data_input, out List<string> zKeys))
                    {
                        MessageBox.Show("Data loaded successfully! Z options: " + string.Join(", ", zKeys));
                        is_data_loaded = true;
                     
                        comboBox_dataheader.Items.AddRange(zKeys.ToArray());
                        comboBox_dataheader.SelectedIndex = 0;
                    }
                    else
                    {
                        MessageBox.Show("Invalid format. Ensure x, y columns and at least one z column.");
                    }

                }
            }

        }

        private void button_importtxt_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                openFileDialog.Title = "Select a TXT file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string file_loc = openFileDialog.FileName;

                    is_data_loaded = false;
                    data_input = new Dictionary<string, List<double>>();
                    data_input = ReadTxt(file_loc);
                    comboBox_dataheader.Items.Clear();

                    if (IsValidContourData(data_input, out List<string> zKeys))
                    {
                        MessageBox.Show("Data loaded successfully! Z options: " + string.Join(", ", zKeys));
                        is_data_loaded = true;
                        
                        comboBox_dataheader.Items.AddRange(zKeys.ToArray());
                        comboBox_dataheader.SelectedIndex = 0;
                    }
                    else
                    {
                        MessageBox.Show("Invalid format. Ensure x, y columns and at least one z column.");
                    }
                }
            }

        }

        private void button_showcontourplot_Click(object sender, EventArgs e)
        {
            if(is_data_loaded == false)
            {
                return;
            }

            // Extract columns
            var x = data_input["x"];
            var y = data_input["y"];

            string result_type = comboBox_dataheader.Items[comboBox_dataheader.SelectedIndex].ToString();

            var z = data_input[result_type];


            // Interpolation
            int gridSize = 200; // Change it to get smoother results?
            var (xi, yi, zi) = InterpolateGrid(x, y, z, gridSize);

            // Create plot model
            var plotModel = new PlotModel { Title = "Contour Plot of " + result_type };

            // Add a HeatMapSeries
            var heatmap = new HeatMapSeries
            {
                X0 = xi[0],
                X1 = xi[xi.Length - 1],
                Y0 = yi[0],
                Y1 = yi[yi.Length - 1],
                Interpolate = true,
                // Palette = OxyPalettes.Rainbow(256),
                Data = zi
            };

            plotModel.Series.Add(heatmap);


            if(checkBox_showcontourline.Checked == true)
            {
                int no_of_contourlines = 0;
                if(comboBox_contourlevel.SelectedIndex == 0)
                {
                    no_of_contourlines = 3;
                }
                else if (comboBox_contourlevel.SelectedIndex == 1)
                {
                    no_of_contourlines = 5;
                }
                else if (comboBox_contourlevel.SelectedIndex == 2)
                {
                    no_of_contourlines = 7;
                }
                else if (comboBox_contourlevel.SelectedIndex == 3)
                {
                    no_of_contourlines = 10;
                }

                // Add ContourSeries for contour lines
                var contourSeries = new ContourSeries
                {
                    ColumnCoordinates = xi,
                    RowCoordinates = yi,
                    Data = zi,
                    LabelBackground = OxyColors.Transparent,
                    FontSize = 10,
                    LabelFormatString = "0.0",
                    StrokeThickness = 1,
                    ContourLevelStep = (zi.Cast<double>().Max() - zi.Cast<double>().Min()) / no_of_contourlines, // 10 contour levels
                                                                                              
                    Color = OxyColors.Black, // Color of contour lines

                };

                plotModel.Series.Add(contourSeries);

            }




            var customPalette = OxyPalette.Interpolate(256, new[]{OxyColors.DarkBlue,
                OxyColors.Blue,
                OxyColors.Cyan,
                OxyColors.LimeGreen,
                OxyColors.Yellow,
                OxyColors.Orange,
                OxyColors.Red,
                OxyColors.DarkRed});


            // Add Color Axis
            var colorAxis = new LinearColorAxis
            {
                Position = AxisPosition.Right,
                Palette = customPalette, // OxyPalettes.Rainbow(256),
                HighColor = OxyColors.Gray, // Optional: handles values above range
                LowColor = OxyColors.Black, // Optional: handles values below range
                Title = result_type
            };

            plotModel.Axes.Add(colorAxis);

            // Add Linear Axes for X and Y
            plotModel.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom, Title = "x" });
            plotModel.Axes.Add(new LinearAxis { Position = AxisPosition.Left, Title = "y" });

            // Show Plot in a Windows Forms Application
            var plotView = new OxyPlot.WindowsForms.PlotView
            {
                Dock = System.Windows.Forms.DockStyle.Fill,
                Model = plotModel
            };

            var form = new System.Windows.Forms.Form { Width = 800, Height = 600 };


            // Set the start position manually
            form.StartPosition = FormStartPosition.Manual;

            // Calculate the center position relative to the main_form
            form.Location = new Point(
                this.Location.X + (this.Width - form.Width) / 2,
                this.Location.Y + (this.Height - form.Height) / 2);

            form.Text = "2D Contour Plot";

            form.Controls.Add(plotView);
            form.Show();
            form.BringToFront();


        }



        private Dictionary<string, List<double>> ReadCsv(string filePath)
        {
            var data = new Dictionary<string, List<double>>();
            using (var reader = new StreamReader(filePath))
            using (var csv = new CsvReader(reader, System.Globalization.CultureInfo.InvariantCulture))
            {
                var records = csv.GetRecords<dynamic>();
                foreach (var record in records)
                {
                    foreach (var pair in record)
                    {
                        if (!data.ContainsKey(pair.Key))
                            data[pair.Key] = new List<double>();
                        data[pair.Key].Add(Convert.ToDouble(pair.Value));
                    }
                }
            }
            return data;
        }


        private Dictionary<string, List<double>> ReadTxt(string filePath)
        {
            var data = new Dictionary<string, List<double>>();
            var lines = File.ReadAllLines(filePath);

            if (lines.Length == 0)
                throw new Exception("File is empty.");

            // Assume first line is header
            var headers = lines[0].Split(new[] { ',', ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);

            foreach (var header in headers)
            {
                data[header] = new List<double>();
            }

            for (int i = 1; i < lines.Length; i++)
            {
                var values = lines[i].Split(new[] { ',', ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (values.Length != headers.Length)
                    throw new Exception($"Mismatch in column count at line {i + 1}");

                for (int j = 0; j < headers.Length; j++)
                {
                    data[headers[j]].Add(Convert.ToDouble(values[j]));
                }
            }

            return data;
        }


        private bool IsValidContourData(Dictionary<string, List<double>> data, out List<string> zKeys)
        {
            zKeys = new List<string>();

            if (data == null || data.Count < 3)
                return false;

            // Ensure x and y exist
            bool hasX = data.Keys.Any(k => k.Trim().ToLower() == "x");
            bool hasY = data.Keys.Any(k => k.Trim().ToLower() == "y");

            if (!hasX || !hasY)
                return false;

            // Get data length from x
            var xKey = data.Keys.First(k => k.Trim().ToLower() == "x");
            int count = data[xKey].Count;

            foreach (var kvp in data)
            {
                if (kvp.Value.Count != count)
                    return false; // Inconsistent row length

                string keyLower = kvp.Key.Trim().ToLower();
                if (keyLower != "x" && keyLower != "y")
                    zKeys.Add(kvp.Key); // Collect Z value headers
            }

            // Ensure at least one Z-value exists
            return zKeys.Count > 0;
        }



        public (double[] xi, double[] yi, double[,] zi) InterpolateGrid(List<double> x, List<double> y, List<double> z, int gridSize)
        {
            var xi = MathNet.Numerics.Generate.LinearSpaced(gridSize, x.Min(), x.Max());
            var yi = MathNet.Numerics.Generate.LinearSpaced(gridSize, y.Min(), y.Max());
            var zi = new double[gridSize, gridSize];


            var interpolator = new DelaunayInterpolator(x, y, z);

            for (int i = 0; i < gridSize; i++)
            {
                for (int j = 0; j < gridSize; j++)
                {
                    // Use Delaunay Interpolator
                    double? z_val = interpolator.Interpolate(xi[i], yi[j]);

                    if (z_val != null)
                    {
                        zi[i, j] = (double)z_val;
                    }
                    else
                    {
                        zi[i, j] = 0.0;
                    }


                }
            }

            return (xi, yi, zi);
        }





    }
}
