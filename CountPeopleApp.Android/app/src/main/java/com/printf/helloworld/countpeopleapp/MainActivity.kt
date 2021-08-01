package com.printf.helloworld.countpeopleapp

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.location.Location
import android.net.ConnectivityManager
import android.net.NetworkInfo
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.MarkerOptions
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import org.json.JSONArray
import org.json.JSONObject
import java.net.HttpURLConnection
import java.net.URL

class MainActivity : AppCompatActivity(), OnMapReadyCallback {
    private val SERVER_URL = "http://121.130.234.20:32734/get"

    private lateinit var map: GoogleMap
    private lateinit var lastLocation: Location
    private lateinit var fusedLocationClient: FusedLocationProviderClient

    companion object {
        private const val LOCATION_REQUEST_CODE = 1
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        createFragment()
        fusedLocationClient = LocationServices.getFusedLocationProviderClient(this)
    }

    override fun onCreateOptionsMenu(menu: Menu?) : Boolean {
        menuInflater.inflate(R.menu.toolbar_menu, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem) : Boolean {
        when (item.itemId) {
            R.id.sync -> syncMap()
        }

        return false
    }

    private fun createFragment() {
        val mapFragment: SupportMapFragment = supportFragmentManager.findFragmentById(R.id.map) as SupportMapFragment
        mapFragment.getMapAsync(this)
    }

    override fun onMapReady(googleMap: GoogleMap) {
        map = googleMap
        map.apply {
            uiSettings.isMapToolbarEnabled = false
            uiSettings.isZoomControlsEnabled = true
        }

        initMap()
        syncMap()
    }

    // 지도 초기화
    private fun initMap() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, arrayOf(android.Manifest.permission.ACCESS_FINE_LOCATION), LOCATION_REQUEST_CODE)
            return
        }

        map.isMyLocationEnabled = true
        fusedLocationClient.lastLocation.addOnSuccessListener(this) { location->
            if (location != null) {
                lastLocation = location
                val pos = LatLng(location.latitude, location.longitude)
                map.moveCamera(CameraUpdateFactory.newLatLng(pos))
                map.animateCamera(CameraUpdateFactory.newLatLngZoom(pos, 15f))
                // GPS 확인
                //Toast.makeText(this, location.latitude.toString() + ", " + location.longitude.toString(), Toast.LENGTH_SHORT).show()
            }
        }
    }

    // 지도 동기화
    private fun syncMap() {
        val cm = getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager
        val activeNetwork: NetworkInfo? = cm.activeNetworkInfo
        val isConnected: Boolean = activeNetwork?.isConnectedOrConnecting == true

        if (!isConnected) {
            Toast.makeText(this, "인터넷에 연결되어 있지 않습니다.", Toast.LENGTH_SHORT).show()
            return
        }

        Toast.makeText(this, "동기화를 하였습니다.", Toast.LENGTH_SHORT).show()

        try {
            CoroutineScope(Dispatchers.Default).launch {
                try {
                    val url = URL(SERVER_URL)
                    var jsonText = ""

                    with(url.openConnection() as HttpURLConnection) {
                        requestMethod = "GET"
                        inputStream.bufferedReader().use {
                            it.lines().forEach { line ->
                                jsonText += line
                            }
                        }
                    }

                    if (jsonText != "") {
                        CoroutineScope(Dispatchers.Main).launch {
                            map.clear()

                            val json = JSONArray(jsonText)
                            for (i in 0 until json.length()) {
                                val obj = json.getJSONObject(i)
                                val locName = obj.optString("locName")
                                val loc1 = obj.optString("loc1").toDouble()
                                val loc2 = obj.optString("loc2").toDouble()
                                val count = obj.optString("count").toInt()

                                addMarker(LatLng(loc1, loc2), locName,"약 " + count.toString() + "명이 있습니다.")
                            }
                        }
                    }
                } catch (e: Exception) {
                    println(e.toString())
                }
            }
        } catch (e: Exception) {
            println(e.toString())
        }
    }

    // 마커 추가
    private fun addMarker(pos: LatLng, title: String, snippet: String) {
        map.addMarker(MarkerOptions()
            .alpha(0.8f)
            .position(pos)
            .title(title)
            .snippet(snippet)
        )
    }
}
