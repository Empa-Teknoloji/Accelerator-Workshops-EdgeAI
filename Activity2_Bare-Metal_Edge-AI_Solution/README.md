<p align="center">
    <img src="./Additionals/Empa-Workshops-Template-Banner2.png" alt="Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

# 2) ST Platformlarında Uçta Yapay Zeka Çözümleri Geliştirme
Empa Electronics tarafından düzenlenen Accelerators Workshops serimizin "Uçta Yapay Zeka - Ankara" adımına hoş geldiniz.  
Bu kılavuz, modern makine öğrenimi kütüphaneleri ve standart yaklaşımlarla oluşturulacak "El Karakterleri Tanıma" uygulamamızın geliştirme adımlarında size rehberlik edecektir.

Aktivite içeriği olan "El Karakteri Tanıma" uygulaması,  workshop için Empa Electronics tarafından tasarlanmış ve üretilmiş Kafa Kart (_Mind Board_) kullanılarak gerçekleştirilecektir. Kart üzerindeki ivmeölçer ve jiroskop sensörlerinden alınan toplam 6 eksenli sensör ölçümleri, bir yapay zeka modeline girdi olarak kullanılacak ve uç birim (Kafa Kart) üzerinde 5 farklı el karakterini sınıflandırmak amacıyla kullanılacaktır. Kafa Kart'ın elde tutulmasıyla gerçekleştirilecek el karakterleri görsel ile açıklanmıştır.

<img src="./Additionals/Hand-Characters.png" alt="Accelerator Workshops" width="800"/> 

## Kurulum
Öncelikle, aşağıdaki bağlantıyı kullanarak çalışma ortamı kurulum adımlarını takip ediniz.
### ↳ [Çalışma Ortamı Kurulumu](Kurulum.md)
Aktivite için gerekli program ve gereçlerin kurulum adımlarını içerir.

## Uygulama
### ↳ [STM32 Platformlarında Uçta Yapay Zeka: El Karakteri Sınıflandırma](https://colab.research.google.com/drive/1rSpHM_JGy5NAFCWkZ7TPcCt1fxc8kywF?usp=sharing)
Aktivite içeriği olan "El Karakteri Tanıma" uygulamasının geliştirme adımlarını içerir.

### ↳ [Model Testi (Yerel): El Karakteri Sınıflandırma](Uygulama_test_local_hand_character_recognition.ipynb)
Aktivite içeriği olan "El Karakteri Tanıma" uygulamasının yerel ortamdaki test adımlarını içerir.

### ↳ [STM32CubeAI Çıktısının MindBoard'a Implementasyonu](Ucbirim_Proje_Kurulum.md)
Aktivite içeriği olan "El Karakteri Tanıma" uygulamasının uç birimde çözüm haline getirilmesi için gerekli implementasyon adımlarını içerir.

### ↳ [Model Testi (Uçbirim): El Karakteri Sınıflandırma](Uygulama_test_edge_hand_character_recognition.ipynb)
Aktivite içeriği olan "El Karakteri Tanıma" uygulamasının uç birimdeki test adımlarını içerir.

## ST Platformlarında Uçta Yapay Zeka

**Tensorflow İle Makine Öğrenimi Modelleri Geliştirme**  
Tensorflow, beraberinde barındırdığı Keras kütüphanesinin de gücüyle, modern derin öğrenme / yapay zeka uygulamalarının geliştirilmesine olanak sağlayan en önemli açık kaynaklı geliştirme kaynaklarından biridir. Tensorflow kütüphanesi, yüksek-seviye kullanım dili ve bu sayede edindiği kullanım kolaylığı sebebiyle workshop etkinliği içerisinde geliştirilecek derin öğrenme modelinin temeli olarak tercih edilmiştir.

**STM32 Cube-AI İle Uçta Yapay Zeka Çözümleri Dağıtımı**  
Geliştirilen bir makine öğrenmesi/derin öğrenme modelinin bir uç birimde, ST platformlarında tesis edilmesi, STMicroelectronics tarafından geliştirilen STM32CubeAI dönüşüm aracı ile sağlanabilmektedir. STM32CubeAI, pek çok popüler model formatı desteği ve dahili olarak gelen model sıkıştırma/derleme araçlarıyla geliştirilen modellerin kolayca çözümleştirilebilmesine imkan tanımaktadır.

**Kaynaklar & Okuma Önerileri** 

1- [Tensorflow2 Quick Start for Beginners](https://www.tensorflow.org/tutorials/quickstart/beginner)

2- [STM32Cube.AI - A Free Tool For Edge-AI Developers](https://stm32ai.st.com/stm32-cube-ai/)