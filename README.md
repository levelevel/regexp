# regexp：正規表現

GNU regex(POSIX)相当の基本正規表現(BRE)と拡張正規表現(ERE)をサポート。

Unicode(UTF-8)にも対応しているが、実用を目指したものではない。

PCRE(PCRE2)の一部対応中。

## 経緯

[A Regular Expression Matcher, Code by Rob Pike](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html)
 という記事で示された簡易正規表現検索コードが非常にシンプルでわかりやすく、美しいといってもよい出来栄えであった。

そのコードでは以下のような簡易的な正規表現をサポートする。
十分ではないが、そこそこ実用的である。

```text
    c    matches any literal character c
    .    matches any single character
    ^    matches the beginning of the input string
    $    matches the end of the input string
    *    matches zero or more occurrences of the previous character
```

40行程度の短いコードであるのでそのすべてを引用すると以下の通りである。

```c
    /* match: search for regexp anywhere in text */
    int match(char *regexp, char *text)
    {
        if (regexp[0] == '^')
            return matchhere(regexp+1, text);
        do {    /* must look even if string is empty */
            if (matchhere(regexp, text))
                return 1;
        } while (*text++ != '\0');
        return 0;
    }

    /* matchhere: search for regexp at beginning of text */
    int matchhere(char *regexp, char *text)
    {
        if (regexp[0] == '\0')
            return 1;
        if (regexp[1] == '*')
            return matchstar(regexp[0], regexp+2, text);
        if (regexp[0] == '$' && regexp[1] == '\0')
            return *text == '\0';
        if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
            return matchhere(regexp+1, text+1);
        return 0;
    }

    /* matchstar: search for c*regexp at beginning of text */
    int matchstar(int c, char *regexp, char *text)
    {
        do {    /* a * matches zero or more instances */
            if (matchhere(regexp, text))
                return 1;
        } while (*text != '\0' && (*text++ == c || c == '.'));
        return 0;
    }
```

御覧の通り非常にシンプルなコードではあるが、要求される機能を完璧に実現している。
とくに最後の `matchstar` は一瞥しただけでは何をやっているのかよくわからないが、
注意深く見てみると驚くほど効率よく繰り返しのマッチングを処理していることがわかる。
テキスト処理と再起処理のお手本といってもよいであろう。

そこで、このコードをベースに、より実用的な正規表現に向けた機能追加をしていこうと思い立って作成したのがこの `regexp` だ。
全体で1000行以上のコードとなってしまったが、オリジナルの `matchstar` の面影も [`reg_match_repeat`](https://github.com/levelevel/regexp/blob/04bdfde000516cbc8ee990de5ddbe533e6a99a44/regexp.c#L1344) にしっかり残っている。

作成に当たって、正規表現の仕様および外部IFに関しては glibc の
 [`libex`](https://linuxjm.osdn.jp/html/LDP_man-pages/man3/regex.3.html) ライブラリを参考にした。
一部の構造体やシンボルもそのまま流用している。

## 実装メモ

### 文字コード

関数の引数に渡す文字はマルチバイト文字(UTF-8)とした。
内部処理も当初はUTF-8のままとしたが、
大文字小文字を区別しない処理ではワイド文字版のtowupper()が必要となるので、
結局ワイド文字で処理するようにした。

### 文字セット

[]で指定する文字セットは、ASCIIのみ対象時は範囲指定やクラス指定も含めて
1バイト文字に対応した256個の配列でマップさせて管理していた。

しかし、マルチバイト文字に対応するにはそうはいかないので、
1バイト文字は従来通り256個の配列マップ、
2バイト以上の文字はリテラルはワイド文字列で保持、
範囲は開始・終了文字を保持、
クラスはその属性を個別に保持してマッチングをとるようにした。

## TODO

- PCRE(PCRE2)
- パターン中の複数文字の連続を文字列単位の処理に変換すると検索効率が上がりそう。

## 参考資料

- [A Regular Expression Matcher, Code by Rob Pike](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html)
- [正規表現 : Wikipedia](https://ja.wikipedia.org/wiki/%E6%AD%A3%E8%A6%8F%E8%A1%A8%E7%8F%BE)
- [UTF-8 : Wikipedia](https://ja.wikipedia.org/wiki/UTF-8)
- [Man page of REGEX](https://linuxjm.osdn.jp/html/LDP_man-pages/man3/regex.3.html)
- [Perl-compatible Regular Expressions (revised API: PCRE2)](https://www.pcre.org/current/doc/html/index.html)
