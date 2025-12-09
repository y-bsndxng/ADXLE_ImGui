# ADXLE_ImGui
**ADXLE_ImGui** は、ADXLE のライブラリを Dear ImGui と ImGuiFileDialog を使った GUI に組み込んだサンプルプロジェクトです。
Dear ImGui ベースのシンプルなウィンドウからファイルダイアログを開き、選択したファイルを ADXLE に渡して処理する最小構成の例になっています。

> ⚠️ **注意**
>
> * 本リポジトリには ADXLE SDK（CRIWARE）の実体は含まれていません。
> * `cri` ディレクトリは ADXLE SDK（ヘッダ・ライブラリ / 付属ファイルなど）を配置するための場所です。
> * ADXLE SDK は各自で正式に入手し、ライセンスに従って使用してください。

---

## Features / 機能

* Dear ImGui ベースのシンプルな GUI
* ImGuiFileDialog によるファイル選択ダイアログ
* 選択したファイルパスを ADXLE に渡すためのラッパー実装
* ADXLE を GUI アプリケーションに統合する際の最小サンプル
* Windows / macOS 向けプロジェクトファイル（`proj` ディレクトリ）

---

## Directory Structure / ディレクトリ構成（概要）

リポジトリ直下の主なディレクトリは次の通りです。

```text
.
├─ cri/          # ADXLE SDK を配置するディレクトリ（中身の構成は環境依存）
├─ external/     # 外部ライブラリ (ImGui / ImGuiFileDialog) ※ Git submodule
├─ proj/         # 各プラットフォーム向けプロジェクトファイル
├─ src/          # アプリケーション本体 & ラッパーコード
├─ LICENSE
└─ README.md
```

* `cri/` と `src/` の詳細なディレクトリ構成やファイル構成は、将来的に変更される可能性があるため、本ドキュメントでは固定的な構造を記載していません。
* 実際の構成はソースコードを参照してください。

---

## Requirements / 動作環境

* C++17 以上をサポートするコンパイラ
* 対応 OS（想定）
  * Windows 10 以降
  * macOS 以降
* グラフィックス
  * Dear ImGui が動作する環境（例：OpenGL + GLFW など）
* 外部ライブラリ / SDK
  * [Dear ImGui](https://github.com/ocornut/imgui)
  * [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
  * [CRI ADX LE](https://game.criware.jp/products/adx-le/) 

> ⚠️ **注意**
>
> * CRI ADX LE は別途入手、配置が必要です。
> * CRI ADX LE Ver. 2.29.01 以降の SDK で開発、動作確認を行っています。

---

## Setup / セットアップ手順

### 1. リポジトリ取得

```bash
git clone https://github.com/yourname/ADXLE_ImGui.git
cd ADXLE_ImGui
```

### 2. submodule (ImGui / ImGuiFileDialog) を取得

```bash
git submodule update --init --recursive
```

* `external` ディレクトリに Dear ImGui および ImGuiFileDialog が取得されます。

### 3. ADXLE SDK の配置
* Native 向け SDK を [CRI ADX LE](https://game.criware.jp/products/adx-le/)より入手してください。
* 入手した ADXLE SDK を `cri` ディレクトリ配下に配置します。
* ヘッダファイルやライブラリファイルの具体的な配置場所は、ご自身の環境・プロジェクト設定に合わせてください。
* 各プロジェクトファイル（`proj` 以下）で、この `cri` ディレクトリをインクルードパス / ライブラリパスとして参照するように設定してください。


## Limitations / 制限事項

* ADXLE SDK を配置していない環境ではビルドできません。
* ADXLE SDK の再配布は行っていないため、このリポジトリ単体ではリンクエラーが発生する場合があります。
* GUI はサンプル用途を想定した最小実装であり、エラーハンドリングや UX は簡易的です。
* ImGui / ImGuiFileDialog / ADXLE SDK のバージョンによっては、API 仕様の違いによりコードの修正が必要になる可能性があります。

---

## License / ライセンス

* 本サンプルコードのライセンスについては、リポジトリ直下の `LICENSE` を参照してください。
* Dear ImGui、ImGuiFileDialog、ADXLE SDK など外部ライブラリ / SDK のライセンスは、それぞれの配布元のライセンスに従ってください。

---

## 備考

* 本リポジトリは「ADXLE を Dear ImGui ベースの GUI から利用する際の最小例」を目的としています。
* 実際のプロダクションコードでは、ログ機構、エラー処理、設定画面などを追加して発展させてください。
* `cri` / `src` の内部構成を変更した場合でも、プロジェクト設定とソースコードが整合していれば、README の記述を大きく変更せずに運用できるような方針で記載しています。
