from cs50 import get_string
import re


def main():
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    formula(letters, words, sentences)


def count_letters(text):
    uppercase_characters = re.findall(r"[A-Z]", text)  # 大文字を検索
    lowercase_characters = re.findall(r"[a-z]", text)  # 小文字を検索
    letters = len(uppercase_characters) + len(lowercase_characters)  # それぞれの文字数を数える
    return letters


def count_words(text):
    space = text.count(" ")  # text中の半角スペースの数を数える
    words = space + 1
    return words


def count_sentences(text):
    sentences = text.count(".") + text.count("!") + text.count("?")  # text中の. ! ?の数を数える
    return sentences


def formula(letters, words, sentences):
    L = (100 * letters / words)
    S = (100 * sentences / words)
    index = round(0.0588 * L - 0.296 * S - 15.8)
    #print(f"letters: {letters} words:{words} sentences{sentences}")

    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


main()
