import tkinter as tk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import messagebox
from tkinter import filedialog
from tkinter import ttk  # Added for themed widgets
from pynput.keyboard import Key, Listener
from syntax import syntax as syn
from tkinter import scrolledtext


def suggest_keywords(prefix):
    suggestions = [keyword for keyword in python_keywords if keyword.startswith(prefix)]

    if suggestions:
        autocomplete(suggestions[0][len(prefix):])


def autocomplete(suggestion):
    if suggestion:
        text_area.insert(tk.END, suggestion)


def on_tab(event):
    current_text = text_area.get("1.0", tk.END).splitlines()
    current_line = current_text[-1]

    if current_line.strip() != "":
        words = current_line.split()
        last_word = words[-1]
        suggest_keywords(last_word)


python_keywords = [
    "False", "None", "True", "and", "as", "assert", "async", "await", "break",
    "class", "continue", "def", "del", "elif", "else", "except", "finally",
    "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal",
    "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"
]


def on_press(key):
    syntax_dictionary = syn()
    highlight_syntax(syntax_dictionary)


root = tk.Tk()
root.title("TBox - By Giovanni Carlino")
root.geometry("1000x600")

style = ttk.Style()
style.theme_use('clam')

text_area = scrolledtext.ScrolledText(root, wrap=tk.WORD, width=40, height=10, font=("Source Code Pro", 12))
text_area.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
text_area.bind("<Tab>", on_tab)  # Bind Tab key to trigger autocompletion

filename = ""


def open_file():
    ofd = filedialog.askopenfile(mode='r', defaultextension=".txt",
                                 filetypes=[("Text files", "*.txt")])
    if ofd:
        global filename
        filename = ofd.name
        content = ofd.read()
        text_area.delete(1.0, tk.END)
        text_area.insert(tk.END, content)
        ofd.close()
        highlight_syntax(syn())


def save_file():
    if filename:
        with open(filename, 'w') as file:
            if len(text_area.get(1.0, tk.END)) > 0:
                file.write(text_area.get(1.0, tk.END))
                messagebox.showinfo("File Saved!", f"Saved {filename} Successfully")
    else:
        save_as()


def save_as():
    sfd = filedialog.asksaveasfile(mode='w', defaultextension=".txt")
    if sfd:
        filename2 = sfd.name
        if len(text_area.get(1.0, tk.END)) > 0:
            sfd.write(text_area.get(1.0, tk.END))
            messagebox.showinfo("File Saved!", f"Saved {filename2} Successfully")
    else:
        return


def highlight_syntax(syntax):
    # Get the whole content in the text area
    content = text_area.get("1.0", tk.END)

    # Apply the syntax highlighting
    for word, color in syntax.items():
        start = "1.0"
        while True:
            start = text_area.search(word, start, tk.END, regexp=True)
            if not start:
                break

            end = f"{start}+{len(word)}c"
            text_area.tag_add("highlight", start, end)
            text_area.tag_config("highlight", foreground=color)
            start = end

    # Highlighting might affect the last character's format, so reset to default format
    text_area.tag_remove("highlight", "end-1c", tk.END)


def create_text_editor():
    menu_bar = Menu(root)
    file_menu = Menu(menu_bar, tearoff=0)
    file_menu.add_command(label="Open", command=open_file)
    file_menu.add_command(label="Save", command=save_file)
    file_menu.add_command(label="Save as", command=save_as)
    file_menu.add_separator()
    file_menu.add_command(label="Exit", command=root.quit)
    menu_bar.add_cascade(label="File", menu=file_menu)

    with Listener(on_press=on_press) as listener:
        root.config(menu=menu_bar)
        root.mainloop()
        # Collect events until released
        listener.join()


if __name__ == "__main__":
    create_text_editor()
