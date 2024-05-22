# texteditor 8
import tkinter as tk
from tkinter import font, filedialog, messagebox, simpledialog

def new_file():
    """Clears the text widget to start a new file."""
    confirmation = messagebox.askyesno("New File", "Are you sure you want to start a new file? Any unsaved changes will be lost.")
    if confirmation:
        text.delete('1.0', tk.END)

def open_file():
    """Opens a file selected by the user and displays its contents in the text widget."""
    file_path = filedialog.askopenfilename()
    if file_path:
        try:
            with open(file_path, 'r') as file:
                text.delete('1.0', tk.END)
                text.insert(tk.END, file.read())
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred while opening the file:\n{str(e)}")

def save_file():
    """Saves the contents of the text widget to a file."""
    file_path = filedialog.asksaveasfilename(defaultextension=".txt")
    if file_path:
        try:
            with open(file_path, 'w') as file:
                file.write(text.get('1.0', tk.END))
            messagebox.showinfo("Success", "File saved successfully.")
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred while saving the file:\n{str(e)}")

def copy_text():
    """Copies the selected text to the clipboard."""
    text.clipboard_clear()
    text.clipboard_append(text.selection_get())

def cut_text():
    """Cuts the selected text and copies it to the clipboard."""
    copy_text()
    text.delete('sel.first', 'sel.last')

def paste_text():
    """Pastes the text from the clipboard into the text widget."""
    text.insert(tk.INSERT, text.clipboard_get())

def find_text():
    """Finds the specified text in the text widget."""
    text.tag_remove('found', '1.0', tk.END)
    search_for = simpledialog.askstring("Find", "Enter text to find:")
    if search_for:
        start = '1.0'
        found_count = 0
        while True:
            start = text.search(search_for, start, stopindex=tk.END)
            if not start:
                break
            found_count += 1
            end = f"{start}+{len(search_for)}c"
            text.tag_add('found', start, end)
            start = end
        text.tag_config('found', foreground='red')
        if found_count == 0:
            messagebox.showinfo("Not Found", "The specified text was not found.")

def replace_text():
    """Replaces the specified text in the text widget."""
    find = simpledialog.askstring("Replace", "Find what:")
    replace_with = simpledialog.askstring("Replace", "Replace with:")
    if find and replace_with:
        all_text = text.get('1.0', tk.END)
        new_text, count = all_text.replace(find, replace_with), all_text.count(find)
        if count == 0:
            messagebox.showinfo("No Replacements", "No replacements were made.")
        else:
            text.delete('1.0', tk.END)
            text.insert(tk.END, new_text)
            messagebox.showinfo("Replacements Made", f"{count} replacements made.")

def undo_text():
    """Undoes the last action in the text widget."""
    try:
        text.edit_undo()
    except tk.TclError:
        pass

def redo_text():
    """Redoes the last undone action in the text widget."""
    try:
        text.edit_redo()
    except tk.TclError:
        pass

root = tk.Tk()
root.title("Simple Text Editor")

text = tk.Text(root, undo=True)  # Enable the undo feature
text.pack(expand=True, fill='both')

menu_bar_font = font.Font(family="Helvetica", size=12)

menu_bar = tk.Menu(root, font=menu_bar_font)

file_menu_font = font.Font(family="Helvetica", size=12, weight="bold")
file_menu = tk.Menu(menu_bar, tearoff=0, font=file_menu_font)
file_menu.add_command(label="New", command=new_file, accelerator="Ctrl+N")
file_menu.add_command(label="Open", command=open_file, accelerator="Ctrl+O")
file_menu.add_command(label="Save", command=save_file, accelerator="Ctrl+S")
file_menu.add_separator()
file_menu.add_command(label="Exit", command=root.quit, accelerator="Ctrl+Q")
menu_bar.add_cascade(label="File", menu=file_menu)

edit_menu_font = font.Font(family="Helvetica", size=12, weight="bold")
edit_menu = tk.Menu(menu_bar, tearoff=0, font=edit_menu_font)
edit_menu.add_command(label="Copy", command=copy_text, accelerator="Ctrl+C")
edit_menu.add_command(label="Cut", command=cut_text, accelerator="Ctrl+X")
edit_menu.add_command(label="Paste", command=paste_text, accelerator="Ctrl+V")
edit_menu.add_separator()
edit_menu.add_command(label="Undo", command=undo_text, accelerator="Ctrl+Z")
edit_menu.add_command(label="Redo", command=redo_text, accelerator="Ctrl+Y")
menu_bar.add_cascade(label="Edit", menu=edit_menu)

find_replace_menu_font = font.Font(family="Helvetica", size=12, weight="bold")
find_replace_menu = tk.Menu(menu_bar, tearoff=0, font=find_replace_menu_font)
find_replace_menu.add_command(label="Find", command=find_text, accelerator="Ctrl+F")
find_replace_menu.add_command(label="Replace", command=replace_text, accelerator="Ctrl+H")
menu_bar.add_cascade(label="Find & Replace", menu=find_replace_menu)

root.config(menu=menu_bar)

root.bind_all("<Control-n>", lambda event: new_file())
root.bind_all("<Control-o>", lambda event: open_file())
root.bind_all("<Control-s>", lambda event: save_file())
root.bind_all("<Control-q>", lambda event: root.quit())
root.bind_all("<Control-f>", lambda event: find_text())
root.bind_all("<Control-h>", lambda event: replace_text())
root.bind_all("<Control-z>", lambda event: undo_text())
root.bind_all("<Control-y>", lambda event: redo_text())

root.mainloop()
