using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GaugeBar : MonoBehaviour
{
    public Image Gauge;

    public void ShowGauge(int current, int max)
    {
        float percent = (float)current / max;
        Gauge.fillAmount = percent;
    }
}
