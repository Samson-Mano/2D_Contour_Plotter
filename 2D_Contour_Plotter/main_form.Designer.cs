namespace _2D_Contour_Plotter
{
    partial class main_form
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(main_form));
            this.button_importcsv = new System.Windows.Forms.Button();
            this.button_importtxt = new System.Windows.Forms.Button();
            this.label_dataheader = new System.Windows.Forms.Label();
            this.comboBox_dataheader = new System.Windows.Forms.ComboBox();
            this.button_showcontourplot = new System.Windows.Forms.Button();
            this.checkBox_showcontourline = new System.Windows.Forms.CheckBox();
            this.comboBox_contourlevel = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // button_importcsv
            // 
            this.button_importcsv.Location = new System.Drawing.Point(106, 30);
            this.button_importcsv.Name = "button_importcsv";
            this.button_importcsv.Size = new System.Drawing.Size(144, 52);
            this.button_importcsv.TabIndex = 0;
            this.button_importcsv.Text = "Import CSV file";
            this.button_importcsv.UseVisualStyleBackColor = true;
            this.button_importcsv.Click += new System.EventHandler(this.button_importcsv_Click);
            // 
            // button_importtxt
            // 
            this.button_importtxt.Location = new System.Drawing.Point(106, 88);
            this.button_importtxt.Name = "button_importtxt";
            this.button_importtxt.Size = new System.Drawing.Size(144, 52);
            this.button_importtxt.TabIndex = 1;
            this.button_importtxt.Text = "Import TXT file";
            this.button_importtxt.UseVisualStyleBackColor = true;
            this.button_importtxt.Click += new System.EventHandler(this.button_importtxt_Click);
            // 
            // label_dataheader
            // 
            this.label_dataheader.AutoSize = true;
            this.label_dataheader.Location = new System.Drawing.Point(20, 167);
            this.label_dataheader.Name = "label_dataheader";
            this.label_dataheader.Size = new System.Drawing.Size(80, 15);
            this.label_dataheader.TabIndex = 2;
            this.label_dataheader.Text = "Data Header: ";
            // 
            // comboBox_dataheader
            // 
            this.comboBox_dataheader.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_dataheader.FormattingEnabled = true;
            this.comboBox_dataheader.Location = new System.Drawing.Point(106, 164);
            this.comboBox_dataheader.Name = "comboBox_dataheader";
            this.comboBox_dataheader.Size = new System.Drawing.Size(254, 23);
            this.comboBox_dataheader.TabIndex = 3;
            // 
            // button_showcontourplot
            // 
            this.button_showcontourplot.Location = new System.Drawing.Point(106, 206);
            this.button_showcontourplot.Name = "button_showcontourplot";
            this.button_showcontourplot.Size = new System.Drawing.Size(144, 52);
            this.button_showcontourplot.TabIndex = 4;
            this.button_showcontourplot.Text = "Show Contour Plot";
            this.button_showcontourplot.UseVisualStyleBackColor = true;
            this.button_showcontourplot.Click += new System.EventHandler(this.button_showcontourplot_Click);
            // 
            // checkBox_showcontourline
            // 
            this.checkBox_showcontourline.AutoSize = true;
            this.checkBox_showcontourline.Checked = true;
            this.checkBox_showcontourline.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_showcontourline.Location = new System.Drawing.Point(286, 206);
            this.checkBox_showcontourline.Name = "checkBox_showcontourline";
            this.checkBox_showcontourline.Size = new System.Drawing.Size(136, 19);
            this.checkBox_showcontourline.TabIndex = 5;
            this.checkBox_showcontourline.Text = "Show Contour Lines";
            this.checkBox_showcontourline.UseVisualStyleBackColor = true;
            // 
            // comboBox_contourlevel
            // 
            this.comboBox_contourlevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_contourlevel.FormattingEnabled = true;
            this.comboBox_contourlevel.Items.AddRange(new object[] {
            "3",
            "5",
            "7",
            "10"});
            this.comboBox_contourlevel.Location = new System.Drawing.Point(286, 231);
            this.comboBox_contourlevel.Name = "comboBox_contourlevel";
            this.comboBox_contourlevel.Size = new System.Drawing.Size(121, 23);
            this.comboBox_contourlevel.TabIndex = 6;
            // 
            // main_form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 301);
            this.Controls.Add(this.comboBox_contourlevel);
            this.Controls.Add(this.checkBox_showcontourline);
            this.Controls.Add(this.button_showcontourplot);
            this.Controls.Add(this.comboBox_dataheader);
            this.Controls.Add(this.label_dataheader);
            this.Controls.Add(this.button_importtxt);
            this.Controls.Add(this.button_importcsv);
            this.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.MaximumSize = new System.Drawing.Size(451, 341);
            this.MinimumSize = new System.Drawing.Size(450, 340);
            this.Name = "main_form";
            this.Text = "2D Contour Plotter";
            this.Load += new System.EventHandler(this.main_form_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_importcsv;
        private System.Windows.Forms.Button button_importtxt;
        private System.Windows.Forms.Label label_dataheader;
        private System.Windows.Forms.ComboBox comboBox_dataheader;
        private System.Windows.Forms.Button button_showcontourplot;
        private System.Windows.Forms.CheckBox checkBox_showcontourline;
        private System.Windows.Forms.ComboBox comboBox_contourlevel;
    }
}

