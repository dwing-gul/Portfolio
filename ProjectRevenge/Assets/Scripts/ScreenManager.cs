using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScreenManager : MonoBehaviour
{
    private void Awake()
    {
        Screen.SetResolution(1600, 900, true);
    }
}
