<p align="center">
    <img src="./Additionals/Empa-Workshops-Template-Banner2.png" alt="Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

# 2) ST Platformlarında Uçta Yapay Zeka Çözümleri Geliştirme

## Çalışma Ortamı Kurulumu
### Bulutta Çalışma Ortamı (Sadece Geliştirme)
Aktivite için oluşturulan bulut çalışma ortamı _geliştirme_ ortamı için ortam kurulumu gerektirmez. Geliştirilen çözümün testi için gerekli test ortamı seri port bağlantısı için yerel çalışma ortamı gerektirir, bulutta çalışma ortamında **kullanılamaz**. Aşağıdaki linke tıklayarak erişebileceğiniz Colab Notebook örneği ile aktivitenin _geliştirme_ adımlarına uygulamalı katılım sağlayabilirsiniz.

**1- Colab Notebook Örneği**  
[Accelerators Workshop - Uçta Yapay Zeka - Aktivite-2 Bulutta Geliştirme Çalışma Ortamı](https://colab.research.google.com/drive/1rSpHM_JGy5NAFCWkZ7TPcCt1fxc8kywF?usp=sharing)

### Yerel Çalışma Ortamı (Sadece Test)

**1- Windows İçin STM32CubeMX & STM32CubeAI**  

Aktivite içeriğinde geliştirilecek çözümün (modelin) ST platformlarına implementasyonu için gerekli STM32CubeMX ve STM32CubeAI ürünlerinin kurulumu adımlarını takip ediniz.

1.1. STM32CubeMX programını aşağıda verilen yönergeleri izleyerek kurunuz.
- Verilen adrese gidiniz ve STM32CubeMX-Lin satırına giderek "Get software" ve sırasıyla "Download as a Guest" butonlarını kullanarak indirme linki talebi oluşturunuz. Ardından e-mail adresinize gelen link ile indirmeyi başlatınız.  
[https://www.st.com/en/development-tools/stm32cubemx.html#get-software](https://www.st.com/en/development-tools/stm32cubemx.html#get-software)
- İndirilen sıkıştırılmış dosyayı mevcut klasör içerisine ayıklayınız.
- Ayıklanan klasör içerisindeki .exe uzantılı "SetupSTM32CubeMX..." dosyasını yönetici olarak çalıştırınız.
- Ardından setup dosyasının çalıştırılmasıyla açılan kurulum yönergesini ön tanımlı seçenekler ile ilerleterek kurulumu tamamlayınız.  
<img src="./Additionals/CubeMX-Setup-Windows.jpeg" alt="Accelerator Workshops" width="600"/>

1.2. STM32CubeAI eklentisini aşağıda verilen yönergeleri izleyerek kurunuz.

STM32CubeMX ürünün kurulumu sonrası STM32CubeAI eklentisinin kurulması için sırasıyla:

* STM32CubeMX programını açtıktan sonra gelen arayüz içerisinde "Manage software installations" bölümünde yer alan "Install / Remove" butonuna tıklayınız.  
<img src="./Additionals/CubeMX-Interface.png" alt="Accelerator Workshops" width="600"/>

* Açılan mini pencere içerisinde STMicroelectronics sekmesi altındaki **"X-CUBE-AI"** paketini bularak güncel sürüm olan **"Artificial Intellicenge 9.0.0"** versiyonunu seçeerek **Install** butonu ile kurulumu başlatınız.
<img src="./Additionals/CubeAI-Package.png" alt="Accelerator Workshops" width="600"/>

* Açılan pencerede ST hesabı ile oturum açma sonrası ön tanımlı değerler ile kurulumu ilerletiniz ve bitiriniz.  
<img src="./Additionals/CubeAI-Version.png" alt="Accelerator Workshops" width="600"/>

**2- Ubuntu İçin STM32CubeMX & STM32CubeAI Kurulumu**  

Aktivite içeriğinde geliştirilecek çözümün (modelin) ST platformlarına implementasyonu için gerekli STM32CubeMX ve STM32CubeAI ürünlerinin kurulumu adımlarını takip ediniz.

2.1. STM32CubeMX programını aşağıda verilen yönergeleri izleyerek kurunuz.
- Verilen adrese gidiniz ve STM32CubeMX-Lin satırına giderek "Get software" ve sırasıyla "Download as a Guest" butonlarını kullanarak indirme linki talebi oluşturunuz. Ardından e-mail adresinize gelen link ile indirmeyi başlatınız.  
[https://www.st.com/en/development-tools/stm32cubemx.html#get-software](https://www.st.com/en/development-tools/stm32cubemx.html#get-software)

- İndirilen sıkıştırılmış dosyayı mevcut klasör içerisine ayıklayınız.
```
sudo apt install unzip
unzip en.stm32cubemx-lin* -d stm32cubemx
```
- Ayrıştırılan klasör içerisine giderek setup için verilen dosya için uygun dosya yetkilendirmesini ayarlayınız.
```
cd stm32cubemx
sudo chmod 777 SetupSTM32CubeMX*
./SetupSTM32CubeMX*
```
- Ayrıştılan klasör içerisindeki setup dosyasını çalıştırınız.
```
./SetupSTM32CubeMX*
```
- Ardından setup dosyasının çalıştırılmasıyla açılan kurulum yönergesini ön tanımlı seçenekler ile ilerleterek kurulumu tamamlayınız.  
<img src="./Additionals/CubeMX-Setup-Linux.png" alt="Accelerator Workshops" height="350"/>

2.2. STM32CubeAI eklentisini aşağıda verilen yönergeleri izleyerek kurunuz.

STM32CubeMX ürünün kurulumu sonrası STM32CubeAI eklentisinin kurulması için sırasıyla:

* STM32CubeMX programını açtıktan sonra gelen arayüz içerisinde "Manage software installations" bölümünde yer alan "Install / Remove" butonuna tıklayınız.  
<img src="./Additionals/CubeMX-Interface.png" alt="Accelerator Workshops" width="600"/>

* Açılan mini pencere içerisinde STMicroelectronics sekmesi altındaki **"X-CUBE-AI"** paketini bularak güncel sürüm olan **"Artificial Intellicenge 9.0.0"** versiyonunu seçeerek **Install** butonu ile kurulumu başlatınız.
<img src="./Additionals/CubeAI-Package.png" alt="Accelerator Workshops" width="600"/>

* Açılan pencerede ST hesabı ile oturum açma sonrası ön tanımlı değerler ile kurulumu ilerletiniz ve bitiriniz.  
<img src="./Additionals/CubeAI-Version.png" alt="Accelerator Workshops" width="600"/>