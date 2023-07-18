namespace launcher
{
    partial class CrasherForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.errorBox = new System.Windows.Forms.RichTextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(112, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(371, 74);
            this.label1.TabIndex = 0;
            this.label1.Text = "Something terrible happened.\r\nIt\'s probably not your fault.\r\n";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Yellow;
            this.panel1.Location = new System.Drawing.Point(46, 108);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(25, 24);
            this.panel1.TabIndex = 1;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Yellow;
            this.panel2.Location = new System.Drawing.Point(46, 22);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(25, 66);
            this.panel2.TabIndex = 2;
            // 
            // errorBox
            // 
            this.errorBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.errorBox.Location = new System.Drawing.Point(119, 108);
            this.errorBox.Name = "errorBox";
            this.errorBox.Size = new System.Drawing.Size(737, 383);
            this.errorBox.TabIndex = 3;
            this.errorBox.Text = "";
            // 
            // button1
            // 
            this.button1.Enabled = false;
            this.button1.Location = new System.Drawing.Point(720, 506);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(136, 40);
            this.button1.TabIndex = 4;
            this.button1.Text = "Submit crash";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // CrasherForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.MidnightBlue;
            this.ClientSize = new System.Drawing.Size(894, 569);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.errorBox);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "CrasherForm";
            this.Text = "Basalt crash program";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RichTextBox errorBox;
        private System.Windows.Forms.Button button1;
    }
}

